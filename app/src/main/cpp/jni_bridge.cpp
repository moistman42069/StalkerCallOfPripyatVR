#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <atomic>
#include <string>
#include "SDL_stub.h"

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Forward declarations from android_main.cpp
extern "C" {
    int android_xray_init(const char* dataPath, const char* externalStoragePath);
    const char* android_xray_last_startup_error();
    void android_xray_destroy();
    void android_xray_set_viewport(int width, int height);
    void android_xray_on_frame();
    void android_xray_pause();
    void android_xray_resume();
}

static std::string g_internalDataPath;
static std::string g_externalStoragePath;
static std::atomic_bool g_initialized{false};
static bool g_surfaceReady = false;
static int g_lastTouchX = 0;
static int g_lastTouchY = 0;
static bool g_touchDown = false;

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeInit(JNIEnv* env, jobject /* this */, jstring internalPath, jstring externalPath) {
    const char* internal = env->GetStringUTFChars(internalPath, nullptr);
    const char* external = env->GetStringUTFChars(externalPath, nullptr);

    g_internalDataPath = internal;
    g_externalStoragePath = external;

    env->ReleaseStringUTFChars(internalPath, internal);
    env->ReleaseStringUTFChars(externalPath, external);

    LOGI("Native init:");
    LOGI("  Internal: %s", g_internalDataPath.c_str());
    LOGI("  External: %s", g_externalStoragePath.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceCreated(JNIEnv*, jobject /* this */) {
    LOGI("=== Surface Created ===");
    g_surfaceReady = true;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_openxray_stalker_MainActivity_nativeSurfaceChanged(JNIEnv*, jobject /* this */, jint width, jint height) {
    LOGI("Surface changed: %dx%d", width, height);

    // Initialize only after GLSurfaceView has made its EGL context current and
    // provided the real framebuffer size.
    android_xray_set_viewport(width, height);
    if (g_surfaceReady && !g_initialized) {
        LOGI("Initializing Call of Pripyat engine...");
        const int result = android_xray_init(
            g_internalDataPath.c_str(),
            g_externalStoragePath.c_str()
        );
        if (result == 0) {
            g_initialized = true;
            LOGI("Call of Pripyat engine initialized");
        } else {
            LOGE("Call of Pripyat engine startup failed: %d", result);
            glViewport(0, 0, width, height);
            return result;
        }
    }

    glViewport(0, 0, width, height);
    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_openxray_stalker_MainActivity_nativeGetStartupError(JNIEnv* env, jobject /* this */) {
    return env->NewStringUTF(android_xray_last_startup_error());
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSendTouch(JNIEnv*, jobject /* this */, jint action, jint x, jint y) {
    if (!g_initialized)
        return;

    SDL_Event motion{};
    motion.type = SDL_MOUSEMOTION;
    motion.motion.windowID = 1;
    motion.motion.which = 0;
    motion.motion.state = g_touchDown ? SDL_BUTTON_LMASK : 0;
    motion.motion.x = x;
    motion.motion.y = y;
    motion.motion.xrel = x - g_lastTouchX;
    motion.motion.yrel = y - g_lastTouchY;
    SDL_PushEvent(&motion);
    g_lastTouchX = x;
    g_lastTouchY = y;

    if (action == 0) { // MotionEvent.ACTION_DOWN
        g_touchDown = true;
        SDL_Event button{};
        button.type = SDL_MOUSEBUTTONDOWN;
        button.button.windowID = 1;
        button.button.which = 0;
        button.button.button = SDL_BUTTON_LEFT;
        button.button.state = SDL_PRESSED;
        button.button.clicks = 1;
        button.button.x = x;
        button.button.y = y;
        SDL_PushEvent(&button);
    } else if (action == 1 || action == 3) { // ACTION_UP or ACTION_CANCEL
        if (g_touchDown) {
            SDL_Event button{};
            button.type = SDL_MOUSEBUTTONUP;
            button.button.windowID = 1;
            button.button.which = 0;
            button.button.button = SDL_BUTTON_LEFT;
            button.button.state = SDL_RELEASED;
            button.button.clicks = 1;
            button.button.x = x;
            button.button.y = y;
            SDL_PushEvent(&button);
        }
        g_touchDown = false;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeSendKey(JNIEnv*, jobject /* this */, jint scancode, jboolean down, jboolean repeat) {
    if (!g_initialized || scancode <= SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES)
        return;

    SDL_Event key{};
    key.type = down ? SDL_KEYDOWN : SDL_KEYUP;
    key.key.windowID = 1;
    key.key.state = down ? SDL_PRESSED : SDL_RELEASED;
    key.key.repeat = repeat ? 1 : 0;
    key.key.keysym.scancode = scancode;
    SDL_PushEvent(&key);
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDrawFrame(JNIEnv*, jobject /* this */) {
    android_xray_on_frame();
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativePause(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Paused ===");
    if (g_initialized) {
        android_xray_pause();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeResume(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Resumed ===");
    if (g_initialized) {
        android_xray_resume();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openxray_stalker_MainActivity_nativeDestroy(JNIEnv*, jobject /* this */) {
    LOGI("=== Application Destroyed ===");
    if (g_initialized) {
        android_xray_destroy();
        g_initialized = false;
    }
    g_surfaceReady = false;
}

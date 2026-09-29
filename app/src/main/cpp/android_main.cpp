#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>

#include <cstdlib>
#include <cstdio>
#include <array>
#include <unistd.h>
#include <exception>
#include <memory>
#include <string>

// Match the OpenXRay engine precompiled-header order: Common exposes the
// global engine API, and Engine.h defines ENGINE_API before device.h pulls in
// statistics and renderer factory declarations.
#include "Common/Common.hpp"
#include "xrCore/xrCore.h"
#include "xrEngine/Engine.h"
#include "xrEngine/device.h"
#include "xrEngine/x_ray.h"
#include "xrGame/xrGame.h"
#include "Include/xrRender/xrRender.h"

#define LOG_TAG "OpenXRay"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace
{
std::string g_dataPath;
std::string g_gameDataPath;
std::string g_lastStartupError;
std::unique_ptr<CApplication> g_application;
bool g_engineInitialized = false;
int g_viewportWidth = 0;
int g_viewportHeight = 0;

const std::array<RendererModule*, 2> g_renderModules = {
    xray::render::render_gl::GetRendererModule(),
    nullptr,
};
}

extern "C" {

int android_xray_init(const char* dataPath, const char* gameDataPath) {
    if (g_engineInitialized)
        return 0;

    g_lastStartupError.clear();
    g_dataPath = dataPath ? dataPath : "";
    g_gameDataPath = gameDataPath ? gameDataPath : "";
    if (g_dataPath.empty() || g_gameDataPath.empty()) {
        g_lastStartupError = "Missing Android data or game-data path.";
        LOGE("%s", g_lastStartupError.c_str());
        return -10;
    }

    const std::string fsGameLtx = g_gameDataPath + "/fsgame.ltx";
    FILE* fsFile = fopen(fsGameLtx.c_str(), "rb");
    if (!fsFile) {
        g_lastStartupError = "Cannot open the filesystem config: " + fsGameLtx;
        LOGE("%s", g_lastStartupError.c_str());
        return -11;
    }
    fclose(fsFile);

    GLint glMajor = 0;
    GLint glMinor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glMinor);
    if (glMajor < 3 || (glMajor == 3 && glMinor < 2)) {
        g_lastStartupError = "This build requires an OpenGL ES 3.2 context; the current surface reports " +
            std::to_string(glMajor) + "." + std::to_string(glMinor) + ".";
        LOGE("%s", g_lastStartupError.c_str());
        return -13;
    }

    SDL_SetAndroidSurfaceSize(g_viewportWidth, g_viewportHeight);
    if (chdir(g_gameDataPath.c_str()) != 0)
        LOGE("Could not set game data working directory: %s", g_gameDataPath.c_str());

    setenv("HOME", g_dataPath.c_str(), 1);
    setenv("USER", "android", 1);
    setenv("TMPDIR", g_dataPath.c_str(), 1);
    setenv("TEMP", g_dataPath.c_str(), 1);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    LOGI("Starting Call of Pripyat; GLES renderer=%s, version=%s",
        renderer ? reinterpret_cast<const char*>(renderer) : "unknown",
        version ? reinterpret_cast<const char*>(version) : "unknown");

    // CApplication performs the real OpenXRay boot sequence: Core, settings,
    // renderer selection, Engine, Device, xrGame, and the game's persistent
    // systems/main menu. The current Android EGL context is provided by
    // GLSurfaceView and shared with the renderer through SDL_stub.h.
    const std::string commandLine =
        "-nosplash -no_gamepad -no_gl_context -noprefetch -nosound -r3 -fsltx " + fsGameLtx;

    try {
        g_application = std::make_unique<CApplication>(commandLine.c_str(), &xrGame, g_renderModules);
        Device.Run();
        // CApplication::Run normally dispatches SDL_WINDOWEVENT_FOCUS_GAINED.
        // Android owns the window and does not generate SDL events, so mark
        // its foreground GLSurfaceView active explicitly before frame work.
        Device.OnWindowActivate(Device.m_sdlWnd, true);
        g_engineInitialized = true;
        LOGI("Call of Pripyat engine startup completed");
        return 0;
    } catch (const std::exception& e) {
        g_lastStartupError = std::string("Call of Pripyat startup failed: ") + e.what();
        LOGE("%s", g_lastStartupError.c_str());
    } catch (...) {
        g_lastStartupError = "Call of Pripyat startup failed with an unknown exception.";
        LOGE("%s", g_lastStartupError.c_str());
    }

    g_application.reset();
    return -12;
}

const char* android_xray_last_startup_error() {
    return g_lastStartupError.c_str();
}

void android_xray_destroy() {
    if (!g_engineInitialized)
        return;

    LOGI("Shutting down Call of Pripyat");
    g_engineInitialized = false;
    Device.Shutdown();
    g_application.reset();
}

void android_xray_set_viewport(int width, int height) {
    g_viewportWidth = width;
    g_viewportHeight = height;
    SDL_SetAndroidSurfaceSize(width, height);

    if (g_engineInitialized && width > 0 && height > 0 &&
        (Device.dwWidth != static_cast<u32>(width) || Device.dwHeight != static_cast<u32>(height))) {
        Device.dwWidth = static_cast<u32>(width);
        Device.dwHeight = static_cast<u32>(height);
        Device.fWidth_2 = width * 0.5f;
        Device.fHeight_2 = height * 0.5f;
        Device.m_rcWindowClient.w = width;
        Device.m_rcWindowClient.h = height;
        Device.m_rcWindowBounds.w = width;
        Device.m_rcWindowBounds.h = height;
    }
}

void android_xray_on_frame() {
    if (!g_engineInitialized || !g_application) {
        glClearColor(0.03f, 0.03f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }

    try {
        Device.ProcessFrame();
    } catch (const std::exception& e) {
        LOGE("Engine frame failed: %s", e.what());
    } catch (...) {
        LOGE("Engine frame failed with an unknown exception");
    }
}

void android_xray_pause() {
    if (g_engineInitialized)
        Device.Pause(true, true, false, "android_pause");
}

void android_xray_resume() {
    if (g_engineInitialized)
        Device.Pause(false, true, false, "android_resume");
}

} // extern "C"

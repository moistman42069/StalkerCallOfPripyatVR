#pragma once

// Minimal SDL stub for Android - provides types and constants used by xrEngine
// Real SDL2 input handling not used on Android (handled via JNI)

#ifdef __cplusplus
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <mutex>
#if defined(__ANDROID__)
#include <EGL/egl.h>
#include <dlfcn.h>
#endif
#else
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// C compatibility: define nullptr as NULL for pure C compilation
#define nullptr NULL
#define inline __inline
#endif

// SDL string functions - map to standard C
#define SDL_strlen strlen
#define SDL_strlcpy(dst, src, size) strncpy(dst, src, size)
#define SDL_free free
#define SDL_malloc malloc

// SDL basic types
typedef uint16_t Uint16;
typedef uint64_t Uint64;
typedef int8_t Sint8;
typedef int16_t Sint16;
typedef int32_t Sint32;
typedef int64_t Sint64;
typedef uint32_t Uint32;
typedef uint8_t Uint8;

// SDL scancode constants - FULL SET from SDL2
#define SDL_NUM_SCANCODES 512
#define SDL_SCANCODE_UNKNOWN 0
#define SDL_SCANCODE_A 4
#define SDL_SCANCODE_B 5
#define SDL_SCANCODE_C 6
#define SDL_SCANCODE_D 7
#define SDL_SCANCODE_E 8
#define SDL_SCANCODE_F 9
#define SDL_SCANCODE_G 10
#define SDL_SCANCODE_H 11
#define SDL_SCANCODE_I 12
#define SDL_SCANCODE_J 13
#define SDL_SCANCODE_K 14
#define SDL_SCANCODE_L 15
#define SDL_SCANCODE_M 16
#define SDL_SCANCODE_N 17
#define SDL_SCANCODE_O 18
#define SDL_SCANCODE_P 19
#define SDL_SCANCODE_Q 20
#define SDL_SCANCODE_R 21
#define SDL_SCANCODE_S 22
#define SDL_SCANCODE_T 23
#define SDL_SCANCODE_U 24
#define SDL_SCANCODE_V 25
#define SDL_SCANCODE_W 26
#define SDL_SCANCODE_X 27
#define SDL_SCANCODE_Y 28
#define SDL_SCANCODE_Z 29
#define SDL_SCANCODE_1 30
#define SDL_SCANCODE_2 31
#define SDL_SCANCODE_3 32
#define SDL_SCANCODE_4 33
#define SDL_SCANCODE_5 34
#define SDL_SCANCODE_6 35
#define SDL_SCANCODE_7 36
#define SDL_SCANCODE_8 37
#define SDL_SCANCODE_9 38
#define SDL_SCANCODE_0 39
#define SDL_SCANCODE_RETURN 40
#define SDL_SCANCODE_ESCAPE 41
#define SDL_SCANCODE_BACKSPACE 42
#define SDL_SCANCODE_TAB 43
#define SDL_SCANCODE_SPACE 44
#define SDL_SCANCODE_MINUS 45
#define SDL_SCANCODE_EQUALS 46
#define SDL_SCANCODE_LEFTBRACKET 47
#define SDL_SCANCODE_RIGHTBRACKET 48
#define SDL_SCANCODE_BACKSLASH 49
#define SDL_SCANCODE_NONUSHASH 50
#define SDL_SCANCODE_SEMICOLON 51
#define SDL_SCANCODE_APOSTROPHE 52
#define SDL_SCANCODE_GRAVE 53
#define SDL_SCANCODE_COMMA 54
#define SDL_SCANCODE_PERIOD 55
#define SDL_SCANCODE_SLASH 56
#define SDL_SCANCODE_CAPSLOCK 57
#define SDL_SCANCODE_F1 58
#define SDL_SCANCODE_F2 59
#define SDL_SCANCODE_F3 60
#define SDL_SCANCODE_F4 61
#define SDL_SCANCODE_F5 62
#define SDL_SCANCODE_F6 63
#define SDL_SCANCODE_F7 64
#define SDL_SCANCODE_F8 65
#define SDL_SCANCODE_F9 66
#define SDL_SCANCODE_F10 67
#define SDL_SCANCODE_F11 68
#define SDL_SCANCODE_F12 69
#define SDL_SCANCODE_PRINTSCREEN 70
#define SDL_SCANCODE_SCROLLLOCK 71
#define SDL_SCANCODE_PAUSE 72
#define SDL_SCANCODE_INSERT 73
#define SDL_SCANCODE_HOME 74
#define SDL_SCANCODE_PAGEUP 75
#define SDL_SCANCODE_DELETE 76
#define SDL_SCANCODE_END 77
#define SDL_SCANCODE_PAGEDOWN 78
#define SDL_SCANCODE_RIGHT 79
#define SDL_SCANCODE_LEFT 80
#define SDL_SCANCODE_DOWN 81
#define SDL_SCANCODE_UP 82
#define SDL_SCANCODE_NUMLOCKCLEAR 83
#define SDL_SCANCODE_KP_DIVIDE 84
#define SDL_SCANCODE_KP_MULTIPLY 85
#define SDL_SCANCODE_KP_MINUS 86
#define SDL_SCANCODE_KP_PLUS 87
#define SDL_SCANCODE_KP_ENTER 88
#define SDL_SCANCODE_KP_1 89
#define SDL_SCANCODE_KP_2 90
#define SDL_SCANCODE_KP_3 91
#define SDL_SCANCODE_KP_4 92
#define SDL_SCANCODE_KP_5 93
#define SDL_SCANCODE_KP_6 94
#define SDL_SCANCODE_KP_7 95
#define SDL_SCANCODE_KP_8 96
#define SDL_SCANCODE_KP_9 97
#define SDL_SCANCODE_KP_0 98
#define SDL_SCANCODE_KP_PERIOD 99
#define SDL_SCANCODE_NONUSBACKSLASH 100
#define SDL_SCANCODE_APPLICATION 101
#define SDL_SCANCODE_POWER 102
#define SDL_SCANCODE_KP_EQUALS 103
#define SDL_SCANCODE_F13 104
#define SDL_SCANCODE_F14 105
#define SDL_SCANCODE_F15 106
#define SDL_SCANCODE_F16 107
#define SDL_SCANCODE_F17 108
#define SDL_SCANCODE_F18 109
#define SDL_SCANCODE_F19 110
#define SDL_SCANCODE_F20 111
#define SDL_SCANCODE_F21 112
#define SDL_SCANCODE_F22 113
#define SDL_SCANCODE_F23 114
#define SDL_SCANCODE_F24 115
#define SDL_SCANCODE_EXECUTE 116
#define SDL_SCANCODE_HELP 117
#define SDL_SCANCODE_MENU 118
#define SDL_SCANCODE_SELECT 119
#define SDL_SCANCODE_STOP 120
#define SDL_SCANCODE_AGAIN 121
#define SDL_SCANCODE_UNDO 122
#define SDL_SCANCODE_CUT 123
#define SDL_SCANCODE_COPY 124
#define SDL_SCANCODE_PASTE 125
#define SDL_SCANCODE_FIND 126
#define SDL_SCANCODE_MUTE 127
#define SDL_SCANCODE_VOLUMEUP 128
#define SDL_SCANCODE_VOLUMEDOWN 129
#define SDL_SCANCODE_KP_COMMA 133
#define SDL_SCANCODE_KP_EQUALSAS400 134
#define SDL_SCANCODE_INTERNATIONAL1 135
#define SDL_SCANCODE_INTERNATIONAL2 136
#define SDL_SCANCODE_INTERNATIONAL3 137
#define SDL_SCANCODE_INTERNATIONAL4 138
#define SDL_SCANCODE_INTERNATIONAL5 139
#define SDL_SCANCODE_INTERNATIONAL6 140
#define SDL_SCANCODE_INTERNATIONAL7 141
#define SDL_SCANCODE_INTERNATIONAL8 142
#define SDL_SCANCODE_INTERNATIONAL9 143
#define SDL_SCANCODE_LANG1 144
#define SDL_SCANCODE_LANG2 145
#define SDL_SCANCODE_LANG3 146
#define SDL_SCANCODE_LANG4 147
#define SDL_SCANCODE_LANG5 148
#define SDL_SCANCODE_LANG6 149
#define SDL_SCANCODE_LANG7 150
#define SDL_SCANCODE_LANG8 151
#define SDL_SCANCODE_LANG9 152
#define SDL_SCANCODE_ALTERASE 153
#define SDL_SCANCODE_SYSREQ 154
#define SDL_SCANCODE_CANCEL 155
#define SDL_SCANCODE_CLEAR 156
#define SDL_SCANCODE_PRIOR 157
#define SDL_SCANCODE_RETURN2 158
#define SDL_SCANCODE_SEPARATOR 159
#define SDL_SCANCODE_OUT 160
#define SDL_SCANCODE_OPER 161
#define SDL_SCANCODE_CLEARAGAIN 162
#define SDL_SCANCODE_CRSEL 163
#define SDL_SCANCODE_EXSEL 164
#define SDL_SCANCODE_KP_00 176
#define SDL_SCANCODE_KP_000 177
#define SDL_SCANCODE_THOUSANDSSEPARATOR 178
#define SDL_SCANCODE_DECIMALSEPARATOR 179
#define SDL_SCANCODE_CURRENCYUNIT 180
#define SDL_SCANCODE_CURRENCYSUBUNIT 181
#define SDL_SCANCODE_KP_LEFTPAREN 182
#define SDL_SCANCODE_KP_RIGHTPAREN 183
#define SDL_SCANCODE_KP_LEFTBRACE 184
#define SDL_SCANCODE_KP_RIGHTBRACE 185
#define SDL_SCANCODE_KP_TAB 186
#define SDL_SCANCODE_KP_BACKSPACE 187
#define SDL_SCANCODE_KP_A 188
#define SDL_SCANCODE_KP_B 189
#define SDL_SCANCODE_KP_C 190
#define SDL_SCANCODE_KP_D 191
#define SDL_SCANCODE_KP_E 192
#define SDL_SCANCODE_KP_F 193
#define SDL_SCANCODE_KP_XOR 194
#define SDL_SCANCODE_KP_POWER 195
#define SDL_SCANCODE_KP_PERCENT 196
#define SDL_SCANCODE_KP_LESS 197
#define SDL_SCANCODE_KP_GREATER 198
#define SDL_SCANCODE_KP_AMPERSAND 199
#define SDL_SCANCODE_KP_DBLAMPERSAND 200
#define SDL_SCANCODE_KP_VERTICALBAR 201
#define SDL_SCANCODE_KP_DBLVERTICALBAR 202
#define SDL_SCANCODE_KP_COLON 203
#define SDL_SCANCODE_KP_HASH 204
#define SDL_SCANCODE_KP_SPACE 205
#define SDL_SCANCODE_KP_AT 206
#define SDL_SCANCODE_KP_EXCLAM 207
#define SDL_SCANCODE_KP_MEMSTORE 208
#define SDL_SCANCODE_KP_MEMRECALL 209
#define SDL_SCANCODE_KP_MEMCLEAR 210
#define SDL_SCANCODE_KP_MEMADD 211
#define SDL_SCANCODE_KP_MEMSUBTRACT 212
#define SDL_SCANCODE_KP_MEMMULTIPLY 213
#define SDL_SCANCODE_KP_MEMDIVIDE 214
#define SDL_SCANCODE_KP_PLUSMINUS 215
#define SDL_SCANCODE_KP_CLEAR 216
#define SDL_SCANCODE_KP_CLEARENTRY 217
#define SDL_SCANCODE_KP_BINARY 218
#define SDL_SCANCODE_KP_OCTAL 219
#define SDL_SCANCODE_KP_DECIMAL 220
#define SDL_SCANCODE_KP_HEXADECIMAL 221
#define SDL_SCANCODE_LCTRL 224
#define SDL_SCANCODE_LSHIFT 225
#define SDL_SCANCODE_LALT 226
#define SDL_SCANCODE_LGUI 227
#define SDL_SCANCODE_RCTRL 228
#define SDL_SCANCODE_RSHIFT 229
#define SDL_SCANCODE_RALT 230
#define SDL_SCANCODE_RGUI 231
#define SDL_SCANCODE_MODE 257
#define SDL_SCANCODE_AUDIONEXT 258
#define SDL_SCANCODE_AUDIOPREV 259
#define SDL_SCANCODE_AUDIOSTOP 260
#define SDL_SCANCODE_AUDIOPLAY 261
#define SDL_SCANCODE_AUDIOMUTE 262
#define SDL_SCANCODE_MEDIASELECT 263
#define SDL_SCANCODE_WWW 264
#define SDL_SCANCODE_MAIL 265
#define SDL_SCANCODE_CALCULATOR 266
#define SDL_SCANCODE_COMPUTER 267
#define SDL_SCANCODE_AC_SEARCH 268
#define SDL_SCANCODE_AC_HOME 269
#define SDL_SCANCODE_AC_BACK 270
#define SDL_SCANCODE_AC_FORWARD 271
#define SDL_SCANCODE_AC_STOP 272
#define SDL_SCANCODE_AC_REFRESH 273
#define SDL_SCANCODE_AC_BOOKMARKS 274
#define SDL_SCANCODE_BRIGHTNESSDOWN 275
#define SDL_SCANCODE_BRIGHTNESSUP 276
#define SDL_SCANCODE_DISPLAYSWITCH 277
#define SDL_SCANCODE_KBDILLUMTOGGLE 278
#define SDL_SCANCODE_KBDILLUMDOWN 279
#define SDL_SCANCODE_KBDILLUMUP 280
#define SDL_SCANCODE_EJECT 281
#define SDL_SCANCODE_SLEEP 282
#define SDL_SCANCODE_APP1 283
#define SDL_SCANCODE_APP2 284
#define SDL_SCANCODE_AUDIOREWIND 285
#define SDL_SCANCODE_AUDIOFASTFORWARD 286


// SDL event types
#define SDL_QUIT 0x100

#define SDL_RELEASED 0
#define SDL_PRESSED 1
#define SDL_BUTTON_LEFT 1
#define SDL_BUTTON_LMASK 0x00000001

// SDL controller axis count
#define SDL_CONTROLLER_AXIS_MAX 6

// SDL system cursor count
#define SDL_NUM_SYSTEM_CURSORS 11

// Forward declarations for SDL types (minimal stubs)
struct SDL_GameController;
struct SDL_Cursor;

// Typedefs for C compatibility (allow using SDL_Window instead of struct SDL_Window)
typedef struct SDL_GameController SDL_GameController;
typedef struct SDL_Cursor SDL_Cursor;
typedef struct SDL_Window {
    int x, y, w, h;
    Uint32 flags;
    Uint32 id;
} SDL_Window;
typedef void* SDL_GLContext;

#ifdef __cplusplus
inline int& SDL_AndroidSurfaceWidth() {
    static int width = 1920;
    return width;
}
inline int& SDL_AndroidSurfaceHeight() {
    static int height = 1080;
    return height;
}
inline SDL_Window* SDL_AndroidMainWindow() {
    static SDL_Window window{0, 0, 1920, 1080, 0, 1};
    return &window;
}
inline void SDL_SetAndroidSurfaceSize(int width, int height) {
    SDL_AndroidSurfaceWidth() = width > 0 ? width : 1920;
    SDL_AndroidSurfaceHeight() = height > 0 ? height : 1080;
    SDL_AndroidMainWindow()->w = SDL_AndroidSurfaceWidth();
    SDL_AndroidMainWindow()->h = SDL_AndroidSurfaceHeight();
}
#endif

// SDL rectangle structure
typedef struct SDL_Rect {
    int x, y;
    int w, h;
} SDL_Rect;

// SDL event structure (minimal stub)
// SDL Keyboard Event structure
// SDL Keysym structure
typedef struct SDL_Keysym {
    int scancode;
    int sym;
    Uint16 mod;
    Uint32 unused;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8 state;
    Uint8 repeat;
    Uint8 padding2;
    Uint8 padding3;
    SDL_Keysym keysym;
} SDL_KeyboardEvent;

// SDL Mouse Motion Event
typedef struct SDL_MouseMotionEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint32 state;
    int x;
    int y;
    int xrel;
    int yrel;
} SDL_MouseMotionEvent;

// SDL Mouse Button Event
typedef struct SDL_MouseButtonEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint8 button;
    Uint8 state;
    Uint8 clicks;
    Uint8 padding1;
    int x;
    int y;
} SDL_MouseButtonEvent;

// SDL Mouse Wheel Event
typedef struct SDL_MouseWheelEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    int x;
    int y;
    Uint32 direction;
    float preciseX;
    float preciseY;
} SDL_MouseWheelEvent;

// SDL Text Input Event
typedef struct SDL_TextInputEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    char text[32];
} SDL_TextInputEvent;

// SDL Text Editing Event
typedef struct SDL_TextEditingEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    char text[32];
    int start;
    int length;
} SDL_TextEditingEvent;

// SDL Controller Axis Event
typedef struct SDL_ControllerAxisEvent {
    Uint32 type;
    Uint32 timestamp;
    int which;
    Uint8 axis;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    int value;
} SDL_ControllerAxisEvent;

// SDL Controller Button Event
typedef struct SDL_ControllerButtonEvent {
    Uint32 type;
    Uint32 timestamp;
    int which;
    Uint8 button;
    Uint8 state;
    Uint8 padding1;
    Uint8 padding2;
} SDL_ControllerButtonEvent;

// SDL Controller Device Event
typedef struct SDL_ControllerDeviceEvent {
    Uint32 type;
    Uint32 timestamp;
    int which;
} SDL_ControllerDeviceEvent;

// SDL Controller Sensor Event
typedef struct SDL_ControllerSensorEvent {
    Uint32 type;
    Uint32 timestamp;
    int which;
    int sensor;
    float data[3];
} SDL_ControllerSensorEvent;

// SDL Controller Touchpad Event
typedef struct SDL_ControllerTouchpadEvent {
    Uint32 type;
    Uint32 timestamp;
    int which;
    int touchpad;
    int finger;
    float x;
    float y;
    float pressure;
} SDL_ControllerTouchpadEvent;

// SDL Window Event structure
typedef struct SDL_WindowEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8 event;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    int data1;
    int data2;
} SDL_WindowEvent;

// SDL Display Event structure
typedef struct SDL_DisplayEvent {
    Uint32 type;
    Uint32 timestamp;
    Uint32 display;
    Uint8 event;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    int data1;
} SDL_DisplayEvent;

// SDL Event union - COMPLETE version with ALL event types
typedef union SDL_Event {
    Uint32 type;
    struct {
        Uint32 type;
        Uint32 timestamp;
    } common;
    SDL_WindowEvent window;
    SDL_DisplayEvent display;
    SDL_KeyboardEvent key;
    SDL_TextEditingEvent edit;
    SDL_TextInputEvent text;
    SDL_MouseMotionEvent motion;
    SDL_MouseButtonEvent button;
    SDL_MouseWheelEvent wheel;
    SDL_ControllerAxisEvent caxis;
    SDL_ControllerButtonEvent cbutton;
    SDL_ControllerDeviceEvent cdevice;
    SDL_ControllerSensorEvent csensor;
    SDL_ControllerTouchpadEvent ctouchpad;
    Uint8 padding[128];
} SDL_Event;

// SDL system cursor enum
typedef enum {
    SDL_SYSTEM_CURSOR_ARROW,
    SDL_SYSTEM_CURSOR_IBEAM,
    SDL_SYSTEM_CURSOR_WAIT,
    SDL_SYSTEM_CURSOR_CROSSHAIR,
    SDL_SYSTEM_CURSOR_WAITARROW,
    SDL_SYSTEM_CURSOR_SIZENWSE,
    SDL_SYSTEM_CURSOR_SIZENESW,
    SDL_SYSTEM_CURSOR_SIZEWE,
    SDL_SYSTEM_CURSOR_SIZENS,
    SDL_SYSTEM_CURSOR_SIZEALL,
    SDL_SYSTEM_CURSOR_NO,
    SDL_SYSTEM_CURSOR_HAND
} SDL_SystemCursor;

// SDL MessageBox types and constants
typedef enum {
    SDL_MESSAGEBOX_ERROR = 0x00000010,
    SDL_MESSAGEBOX_WARNING = 0x00000020,
    SDL_MESSAGEBOX_INFORMATION = 0x00000040
} SDL_MessageBoxFlags;

typedef enum {
    SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001,
    SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002
} SDL_MessageBoxButtonFlags;

typedef struct SDL_MessageBoxButtonData {
    Uint32 flags;
    int buttonid;
    const char* text;
} SDL_MessageBoxButtonData;

typedef struct SDL_MessageBoxData {
    Uint32 flags;
    SDL_Window* window;
    const char* title;
    const char* message;
    int numbuttons;
    const SDL_MessageBoxButtonData* buttons;
    const void* colorScheme;
} SDL_MessageBoxData;

// SDL Assertion types and constants
typedef enum {
    SDL_ASSERTION_RETRY = 0,
    SDL_ASSERTION_BREAK = 1,
    SDL_ASSERTION_ABORT = 2,
    SDL_ASSERTION_IGNORE = 3,
    SDL_ASSERTION_ALWAYS_IGNORE = 4
} SDL_AssertState;

typedef struct SDL_AssertData {
    int always_ignore;
    unsigned int trigger_count;
    const char* condition;
    const char* filename;
    int linenum;
    const char* function;
    const struct SDL_AssertData* next;
} SDL_AssertData;

typedef SDL_AssertState (*SDL_AssertionHandler)(const SDL_AssertData* data, void* userdata);

// SDL Log priority
typedef enum {
    SDL_LOG_CATEGORY_APPLICATION = 0,
    SDL_LOG_CATEGORY_ERROR = 1,
    SDL_LOG_CATEGORY_ASSERT = 2,
    SDL_LOG_CATEGORY_SYSTEM = 3,
    SDL_LOG_CATEGORY_AUDIO = 4,
    SDL_LOG_CATEGORY_VIDEO = 5,
    SDL_LOG_CATEGORY_RENDER = 6,
    SDL_LOG_CATEGORY_INPUT = 7,
    SDL_LOG_CATEGORY_TEST = 8,
    SDL_LOG_CATEGORY_RESERVED1 = 9,
    SDL_LOG_CATEGORY_RESERVED2 = 10,
    SDL_LOG_CATEGORY_RESERVED3 = 11,
    SDL_LOG_CATEGORY_RESERVED4 = 12,
    SDL_LOG_CATEGORY_RESERVED5 = 13,
    SDL_LOG_CATEGORY_RESERVED6 = 14,
    SDL_LOG_CATEGORY_RESERVED7 = 15,
    SDL_LOG_CATEGORY_RESERVED8 = 16,
    SDL_LOG_CATEGORY_RESERVED9 = 17,
    SDL_LOG_CATEGORY_RESERVED10 = 18,
    SDL_LOG_CATEGORY_CUSTOM = 19
} SDL_LogCategory;

typedef enum {
    SDL_LOG_PRIORITY_VERBOSE = 1,
    SDL_LOG_PRIORITY_DEBUG = 2,
    SDL_LOG_PRIORITY_INFO = 3,
    SDL_LOG_PRIORITY_WARN = 4,
    SDL_LOG_PRIORITY_ERROR = 5,
    SDL_LOG_PRIORITY_CRITICAL = 6,
    SDL_NUM_LOG_PRIORITIES = 7
} SDL_LogPriority;

// SDL_GetPrefPath - returns Android-specific user data directory
inline char* SDL_GetPrefPath(const char* org, const char* app) {
    const char* path = "/sdcard/Android/data/com.openxray.stalker/files/";
    char* result = (char*)malloc(strlen(path) + 1);
    if (result) {
        strcpy(result, path);
    }
    return result;
}

// SDL_PumpEvents - stub for Android (no-op)
inline void SDL_PumpEvents() {
    // No-op on Android - event handling done via JNI
}

// GLSurfaceView/JNI feeds Android touch and keyboard events through this queue.
#ifdef __cplusplus
inline std::deque<SDL_Event>& SDL_AndroidEventQueue() {
    static std::deque<SDL_Event> events;
    return events;
}
inline std::mutex& SDL_AndroidEventMutex() {
    static std::mutex mutex;
    return mutex;
}
#endif

// SDL_PushEvent - enqueue an event for OpenXRay's input update.
inline int SDL_PushEvent(SDL_Event* event) {
    if (!event)
        return -1;
#ifdef __cplusplus
    std::lock_guard<std::mutex> lock(SDL_AndroidEventMutex());
    auto& events = SDL_AndroidEventQueue();
    if (events.size() >= 4096)
        events.pop_front();
    events.push_back(*event);
    return 1;
#else
    return 0;
#endif
}

// SDL_CreateWindow - stub for Android (returns nullptr)
inline SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
#ifdef __cplusplus
    // GLSurfaceView owns the actual Android window and EGL context. OpenXRay
    // still needs a non-null SDL handle for renderer bookkeeping.
    SDL_Window* window = SDL_AndroidMainWindow();
    window->x = x;
    window->y = y;
    window->w = SDL_AndroidSurfaceWidth();
    window->h = SDL_AndroidSurfaceHeight();
    // SHOWN | OPENGL | INPUT_FOCUS; Android activity owns the foreground
    // surface and does not send SDL_WINDOWEVENT_FOCUS_GAINED.
    window->flags = flags | 0x00000004 | 0x00000002 | 0x00000200;
    window->id = 1;
    return window;
#else
    return NULL;
#endif
}

// SDL_DestroyWindow - stub for Android (no-op)
inline void SDL_DestroyWindow(SDL_Window* window) {
    // No-op on Android - window management done via JNI
}

// SDL_GetError - stub for Android (returns empty string)
inline const char* SDL_GetError() {
    return "";
}

// SDL_HasClipboardText - stub for Android (returns false)
inline bool SDL_HasClipboardText() {
    return false;
}

// SDL_GetClipboardText - stub for Android (returns empty string)
inline char* SDL_GetClipboardText() {
    char* result = (char*)malloc(1);
    if (result) result[0] = '\0';
    return result;
}

// SDL_SetClipboardText - stub for Android (returns 0)
inline int SDL_SetClipboardText(const char* text) {
    // No-op on Android - clipboard handled via JNI
    return 0;
}

// SDL_ShowMessageBox - stub for Android (returns 0)
inline int SDL_ShowMessageBox(const SDL_MessageBoxData* messageboxdata, int* buttonid) {
    // No-op on Android - message boxes handled via JNI
    if (buttonid) *buttonid = 0;
    return 0;
}

// SDL_SetAssertionHandler - stub for Android (no-op)
inline void SDL_SetAssertionHandler(SDL_AssertionHandler handler, void* userdata) {
    // No-op on Android - assertions handled differently
}

// SDL_GetPlatform - returns "Android"
inline const char* SDL_GetPlatform() {
    return "Android";
}

// SDL_GetBasePath - returns Android app directory
inline char* SDL_GetBasePath() {
    const char* path = "/sdcard/Android/data/com.openxray.stalker/files/";
    char* result = (char*)malloc(strlen(path) + 1);
    if (result) {
        strcpy(result, path);
    }
    return result;
}

// SDL_ShowSimpleMessageBox - stub for Android (returns 0)
inline int SDL_ShowSimpleMessageBox(Uint32 flags, const char* title, const char* message, SDL_Window* window) {
    // No-op on Android - message boxes handled via JNI
    return 0;
}

// SDL_arraysize - macro for array size
#define SDL_arraysize(array) (sizeof(array)/sizeof(array[0]))

// SDL_LogSetOutputFunction - stub for Android (no-op)
typedef void (*SDL_LogOutputFunction)(void* userdata, int category, SDL_LogPriority priority, const char* message);
inline void SDL_LogSetOutputFunction(SDL_LogOutputFunction callback, void* userdata) {
    // No-op on Android - logging handled differently
}

// SDL dynamic library loading functions - stubs for Android
// On Android, these are not used (modules are statically linked)
inline void* SDL_LoadObject(const char* sofile) {
    // Return nullptr - dynamic loading not supported on Android in this build
    return nullptr;
}

inline void* SDL_LoadFunction(void* handle, const char* name) {
    // Return nullptr - dynamic loading not supported
    return nullptr;
}

inline void SDL_UnloadObject(void* handle) {
    // No-op - nothing to unload
}

// Additional SDL types needed by xrEngine
typedef struct SDL_Point {
    int x, y;
} SDL_Point;

typedef struct SDL_Surface {
    Uint32 flags;
    void* format;
    int w, h;
    int pitch;
    void* pixels;
    void* userdata;
    int locked;
    void* list_blitmap;
    SDL_Rect clip_rect;
    void* map;
    int refcount;
} SDL_Surface;

typedef enum {
    SDL_HITTEST_NORMAL,
    SDL_HITTEST_DRAGGABLE,
    SDL_HITTEST_RESIZE_TOPLEFT,
    SDL_HITTEST_RESIZE_TOP,
    SDL_HITTEST_RESIZE_TOPRIGHT,
    SDL_HITTEST_RESIZE_RIGHT,
    SDL_HITTEST_RESIZE_BOTTOMRIGHT,
    SDL_HITTEST_RESIZE_BOTTOM,
    SDL_HITTEST_RESIZE_BOTTOMLEFT,
    SDL_HITTEST_RESIZE_LEFT
} SDL_HitTestResult;

// SDL window flags
#define SDL_WINDOW_FULLSCREEN 0x00000001
#define SDL_WINDOW_OPENGL 0x00000002
#define SDL_WINDOW_SHOWN 0x00000004
#define SDL_WINDOW_HIDDEN 0x00000008
#define SDL_WINDOW_BORDERLESS 0x00000010
#define SDL_WINDOW_RESIZABLE 0x00000020
#define SDL_WINDOW_MINIMIZED 0x00000040
#define SDL_WINDOW_MAXIMIZED 0x00000080

// SDL functions that are not used on Android (stubs)
inline SDL_Surface* SDL_LoadBMP(const char* file) {
    // Not used on Android
    return nullptr;
}

inline void SDL_SetWindowMinimumSize(SDL_Window* window, int min_w, int min_h) {
    // No-op on Android
}

inline void SDL_SetTextInputRect(const SDL_Rect* rect) {
    // No-op on Android
}

// SDL boolean values
#define SDL_TRUE 1
#define SDL_FALSE 0

// SDL enable/disable
#define SDL_DISABLE 0
#define SDL_ENABLE 1

// SDL_DisplayMode structure
typedef struct SDL_DisplayMode {
    Uint32 format;
    int w;
    int h;
    int refresh_rate;
    void* driverdata;
} SDL_DisplayMode;

// Additional SDL window and display functions (all stubs for Android)
inline void SDL_SetWindowIcon(SDL_Window* window, SDL_Surface* icon) {
    // No-op on Android
}

typedef SDL_HitTestResult (*SDL_HitTest)(SDL_Window* win, const SDL_Point* area, void* data);

inline int SDL_SetWindowHitTest(SDL_Window* window, SDL_HitTest callback, void* callback_data) {
    // No-op on Android, return 0 (success)
    return 0;
}

inline int SDL_GetNumDisplayModes(int displayIndex) {
    // Return 1 (single mode) for Android
    return 1;
}

inline int SDL_GetDisplayBounds(int displayIndex, SDL_Rect* rect) {
    // Stub: return fake bounds
    if (rect) {
        rect->x = 0;
        rect->y = 0;
        rect->w = 1920;
        rect->h = 1080;
    }
    return 0;
}

inline int SDL_GetDisplayUsableBounds(int displayIndex, SDL_Rect* rect) {
    // Same as GetDisplayBounds on Android
    return SDL_GetDisplayBounds(displayIndex, rect);
}

inline int SDL_GetDisplayDPI(int displayIndex, float* ddpi, float* hdpi, float* vdpi) {
    // Stub: return fake DPI
    if (ddpi) *ddpi = 160.0f;
    if (hdpi) *hdpi = 160.0f;
    if (vdpi) *vdpi = 160.0f;
    return 0;
}

inline int SDL_GetNumVideoDisplays() {
    // Return 1 display for Android
    return 1;
}

inline const char* SDL_GetDisplayName(int displayIndex) {
    // Return fake display name
    return "Android Display";
}

inline Uint32 SDL_GetWindowFlags(SDL_Window* window) {
    return window ? window->flags : SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
}

inline int SDL_SetWindowOpacity(SDL_Window* window, float opacity) {
    // No-op on Android, return 0 (success)
    return 0;
}

inline int SDL_GetWindowDisplayIndex(SDL_Window* window) {
    // Always return 0 (first display) on Android
    return 0;
}

inline void SDL_SetWindowPosition(SDL_Window* window, int x, int y) {
    // No-op on Android
}

inline void SDL_SetWindowSize(SDL_Window* window, int w, int h) {
#ifdef __cplusplus
    if (window) {
        // The Android surface is owned and sized by GLSurfaceView. OpenXRay's
        // desktop window configuration must not resize the Quest framebuffer.
        window->w = SDL_AndroidSurfaceWidth();
        window->h = SDL_AndroidSurfaceHeight();
    }
#endif
}

inline void SDL_GetWindowSize(SDL_Window* window, int* w, int* h) {
    if (w) *w = window ? window->w : 1920;
    if (h) *h = window ? window->h : 1080;
}

inline void SDL_GetWindowPosition(SDL_Window* window, int* x, int* y) {
    // Return fake position
    if (x) *x = 0;
    if (y) *y = 0;
}

inline int SDL_GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode* mode) {
    // Return fake display mode
    if (mode) {
        mode->format = 0;
#ifdef __cplusplus
        mode->w = SDL_AndroidSurfaceWidth();
        mode->h = SDL_AndroidSurfaceHeight();
#else
        mode->w = 1920;
        mode->h = 1080;
#endif
        mode->refresh_rate = 60;
        mode->driverdata = nullptr;
    }
    return 0;
}

inline int SDL_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode* mode) {
    // Same as GetDisplayMode
    return SDL_GetDisplayMode(displayIndex, 0, mode);
}

inline int SDL_GetWindowDisplayMode(SDL_Window* window, SDL_DisplayMode* mode) {
    // Return fake mode
    return SDL_GetDisplayMode(0, 0, mode);
}

inline int SDL_SetWindowDisplayMode(SDL_Window* window, const SDL_DisplayMode* mode) {
    // No-op on Android, return 0 (success)
    return 0;
}

inline int SDL_SetWindowFullscreen(SDL_Window* window, Uint32 flags) {
    // No-op on Android, return 0 (success)
    return 0;
}

typedef int SDL_bool;

inline void SDL_SetWindowBordered(SDL_Window* window, SDL_bool bordered) {
    // No-op on Android
}

inline void SDL_SetWindowResizable(SDL_Window* window, SDL_bool resizable) {
    // No-op on Android
}

inline int SDL_GetWindowBordersSize(SDL_Window* window, int* top, int* left, int* bottom, int* right) {
    // Return zero borders for Android
    if (top) *top = 0;
    if (left) *left = 0;
    if (bottom) *bottom = 0;
    if (right) *right = 0;
    return 0;
}

// Additional SDL window flag
#define SDL_WINDOW_FULLSCREEN_DESKTOP 0x00001001

// SDL pixel format constant
#define SDL_PIXELFORMAT_UNKNOWN 0

// Additional SDL display and window functions
inline SDL_DisplayMode* SDL_GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode* mode, SDL_DisplayMode* closest) {
    // Return the requested mode as closest
    if (closest && mode) {
        *closest = *mode;
    }
    return closest;
}

inline void SDL_SetWindowAlwaysOnTop(SDL_Window* window, SDL_bool on_top) {
    // No-op on Android
}

inline void SDL_ShowWindow(SDL_Window* window) {
    // No-op on Android
}

inline void SDL_MinimizeWindow(SDL_Window* window) {
    // No-op on Android
}

inline void SDL_HideWindow(SDL_Window* window) {
    // No-op on Android
}

// Window flags
#define SDL_WINDOW_INPUT_FOCUS 0x00000200

// Event types
#define SDL_DISPLAYEVENT 0x150
#define SDL_WINDOWEVENT 0x200
#define SDL_PEEKEVENT 2

// Display event types
#define SDL_DISPLAYEVENT_ORIENTATION 0
#define SDL_DISPLAYEVENT_CONNECTED 1
#define SDL_DISPLAYEVENT_DISCONNECTED 2

// Window event types
#define SDL_WINDOWEVENT_MOVED 4
#define SDL_WINDOWEVENT_RESIZED 5
#define SDL_WINDOWEVENT_SIZE_CHANGED 6
#define SDL_WINDOWEVENT_MINIMIZED 7
#define SDL_WINDOWEVENT_MAXIMIZED 8
#define SDL_WINDOWEVENT_RESTORED 9
#define SDL_WINDOWEVENT_SHOWN 1
#define SDL_WINDOWEVENT_HIDDEN 2
#define SDL_WINDOWEVENT_FOCUS_GAINED 10
#define SDL_WINDOWEVENT_FOCUS_LOST 11
#define SDL_WINDOWEVENT_CLOSE 14
#define SDL_WINDOWEVENT_ENTER 11
#define SDL_WINDOWEVENT_LEAVE 12
#define SDL_WINDOWEVENT_DISPLAY_CHANGED 13

// Window position constant
#define SDL_WINDOWPOS_CENTERED 0x2FFF0000

// Window flags (add to existing)
#define SDL_WINDOW_ALWAYS_ON_TOP 0x00008000

// Init flags
#define SDL_INIT_VIDEO 0x00000020
#define SDL_INIT_GAMECONTROLLER 0x00002000

// Event types (add to existing)
#define SDL_GETEVENT 0x800

// Hints
#define SDL_HINT_GRAB_KEYBOARD "SDL_GRAB_KEYBOARD"

// Keyboard modifier flags
#define KMOD_CAPS 0x2000

// SDL initialization and quit functions
inline int SDL_Init(Uint32 flags) {
    return 0; // Success stub
}

inline void SDL_Quit() {
    // No-op on Android stub
}

inline int SDL_QuitRequested() {
    return 0; // Not quitting
}

// Keyboard mod state functions
inline int SDL_GetModState() {
    return 0; // No modifier keys on Android stub
}

inline const char* SDL_GetHint(const char* name) {
    return nullptr; // No hints on Android stub
}

inline void SDL_StopTextInput() {
    // No-op on Android stub
}

inline void* SDL_GetWindowSurface(SDL_Window* window) {
    return nullptr; // No surface on Android stub
}

inline void SDL_UpdateWindowSurface(SDL_Window* window) {
    // No-op on Android stub
}

inline void SDL_RaiseWindow(SDL_Window* window) {
    // No-op on Android stub
}

inline SDL_Window* SDL_GetWindowFromID(Uint32 id) {
#ifdef __cplusplus
    return id == 1 ? SDL_AndroidMainWindow() : nullptr;
#else
    return NULL;
#endif
}

inline int SDL_BlitSurface(void* src, void* srcrect, void* dst, void* dstrect) {
    return 0; // No-op on Android stub
}

inline void SDL_FreeSurface(void* surface) {
    // No-op on Android stub
}

inline int SDL_PeepEvents(SDL_Event* events, int numevents, int action, Uint32 minType, Uint32 maxType) {
#ifdef __cplusplus
    if (action != SDL_GETEVENT && action != SDL_PEEKEVENT)
        return -1;
    std::lock_guard<std::mutex> lock(SDL_AndroidEventMutex());
    auto& queue = SDL_AndroidEventQueue();
    int count = 0;
    for (auto it = queue.begin(); it != queue.end();) {
        if (it->type >= minType && it->type <= maxType) {
            if (events && count < numevents)
                events[count] = *it;
            ++count;
            if (action == SDL_GETEVENT && events && count <= numevents)
                it = queue.erase(it);
            else
                ++it;
            if (events && count >= numevents)
                break;
        } else {
            ++it;
        }
    }
    return count;
#else
    return 0;
#endif
}

// OpenXRay's GL renderer shares the EGL context created by GLSurfaceView.
#define SDL_GL_RED_SIZE 0
#define SDL_GL_GREEN_SIZE 1
#define SDL_GL_BLUE_SIZE 2
#define SDL_GL_ALPHA_SIZE 3
#define SDL_GL_DOUBLEBUFFER 5
#define SDL_GL_DEPTH_SIZE 6
#define SDL_GL_STENCIL_SIZE 7
#define SDL_GL_CONTEXT_MAJOR_VERSION 17
#define SDL_GL_CONTEXT_MINOR_VERSION 18
#define SDL_GL_CONTEXT_PROFILE_MASK 21
#define SDL_GL_CONTEXT_PROFILE_CORE 1

#ifdef __cplusplus
inline int SDL_GL_SetAttribute(int attribute, int value) { return 0; }
inline SDL_GLContext SDL_GL_CreateContext(SDL_Window* window) {
#if defined(__ANDROID__)
    return eglGetCurrentContext() == EGL_NO_CONTEXT ? nullptr : reinterpret_cast<void*>(1);
#else
    return reinterpret_cast<void*>(1);
#endif
}
inline int SDL_GL_MakeCurrent(SDL_Window* window, SDL_GLContext context) {
#if defined(__ANDROID__)
    return context && eglGetCurrentContext() == EGL_NO_CONTEXT ? -1 : 0;
#else
    return 0;
#endif
}
inline SDL_GLContext SDL_GL_GetCurrentContext() {
#if defined(__ANDROID__)
    return eglGetCurrentContext() == EGL_NO_CONTEXT ? nullptr : reinterpret_cast<void*>(1);
#else
    return reinterpret_cast<void*>(1);
#endif
}
inline void SDL_GL_DeleteContext(SDL_GLContext context) {}
inline int SDL_GL_SetSwapInterval(int interval) { return 0; }
inline void SDL_GL_SwapWindow(SDL_Window* window) {}
inline void* SDL_GL_GetProcAddress(const char* name) {
#if defined(__ANDROID__)
    void* proc = dlsym(RTLD_DEFAULT, name);
    if (!proc)
        proc = reinterpret_cast<void*>(eglGetProcAddress(name));
    return proc;
#else
    return nullptr;
#endif
}
#endif

// Add ALL remaining missing SDL constants and functions

// Keyboard and mouse event types
#define SDL_KEYDOWN 0x300
#define SDL_KEYUP 0x301
#define SDL_TEXTEDITING 0x302
#define SDL_TEXTINPUT 0x303
#define SDL_KEYMAPCHANGED 0x304
#define SDL_MOUSEMOTION 0x400
#define SDL_MOUSEBUTTONDOWN 0x401
#define SDL_MOUSEBUTTONUP 0x402
#define SDL_MOUSEWHEEL 0x403

// Controller event types
#define SDL_CONTROLLERAXISMOTION 0x650
#define SDL_CONTROLLERBUTTONDOWN 0x651
#define SDL_CONTROLLERBUTTONUP 0x652
#define SDL_CONTROLLERDEVICEADDED 0x653
#define SDL_CONTROLLERDEVICEREMOVED 0x654
#define SDL_CONTROLLERDEVICEREMAPPED 0x655
#define SDL_CONTROLLERTOUCHPADUP 0x658
#define SDL_CONTROLLERSENSORUPDATE 0x659

// Sensor types
#define SDL_SENSOR_GYRO 1

// Joystick constants
#define SDL_JOYSTICK_AXIS_MAX 6

// Scancode type
typedef int SDL_Scancode;

// Hints
#define SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4 "SDL_WINDOWS_NO_CLOSE_ON_ALT_F4"

// Mouse and keyboard functions
inline int SDL_GetMouseState(int* x, int* y) {
    if (x) *x = 0;
    if (y) *y = 0;
    return 0;
}

inline void SDL_StartTextInput() {
    // No-op on Android stub
}

inline int SDL_SetRelativeMouseMode(int enabled) {
    return 0; // Success stub
}

inline void SDL_WarpMouseInWindow(SDL_Window* window, int x, int y) {
    // No-op on Android stub
}

inline void SDL_SetWindowGrab(SDL_Window* window, int grabbed) {
    // No-op on Android stub
}

inline int SDL_ShowCursor(int toggle) {
    return 1; // Always shown
}

// Cursor functions
inline SDL_Cursor* SDL_CreateSystemCursor(int id) {
    return nullptr; // No cursor on Android stub
}

inline void SDL_SetCursor(SDL_Cursor* cursor) {
    // No-op on Android stub
}

inline void SDL_FreeCursor(SDL_Cursor* cursor) {
    // No-op on Android stub
}

// Game controller functions
inline int SDL_NumJoysticks() {
    return 0; // No joysticks on Android stub
}

inline int SDL_IsGameController(int device_index) {
    return 0; // Not a controller
}

inline SDL_GameController* SDL_GameControllerOpen(int device_index) {
    return nullptr; // No controller on Android stub
}

inline void SDL_GameControllerClose(SDL_GameController* gamecontroller) {
    // No-op on Android stub
}

inline SDL_GameController* SDL_GameControllerFromInstanceID(int joyid) {
    return nullptr; // No controller on Android stub
}

// Additional SDL functions for xrEngine compatibility

inline void SDL_FlushEvent(Uint32 type) {
#ifdef __cplusplus
    std::lock_guard<std::mutex> lock(SDL_AndroidEventMutex());
    auto& queue = SDL_AndroidEventQueue();
    for (auto it = queue.begin(); it != queue.end();) {
        if (it->type == type)
            it = queue.erase(it);
        else
            ++it;
    }
#endif
}

inline void SDL_FlushEvents(Uint32 minType, Uint32 maxType) {
#ifdef __cplusplus
    std::lock_guard<std::mutex> lock(SDL_AndroidEventMutex());
    auto& queue = SDL_AndroidEventQueue();
    for (auto it = queue.begin(); it != queue.end();) {
        if (it->type >= minType && it->type <= maxType)
            it = queue.erase(it);
        else
            ++it;
    }
#endif
}

inline int SDL_GameControllerRumble(SDL_GameController* gamecontroller, Uint16 low_frequency_rumble, 
                                   Uint16 high_frequency_rumble, Uint32 duration_ms) {
    return -1; // Not supported on Android stub
}

inline int SDL_GameControllerRumbleTriggers(SDL_GameController* gamecontroller, Uint16 left_rumble, 
                                           Uint16 right_rumble, Uint32 duration_ms) {
    return -1; // Not supported on Android stub
}

inline int SDL_GameControllerSetSensorEnabled(SDL_GameController* gamecontroller, int type, int enabled) {
    return -1; // Not supported on Android stub
}


inline const char* SDL_GetKeyName(int key) {
    static char buf[32];
    snprintf(buf, sizeof(buf), "Key_%d", key);
    return buf;
}

inline int SDL_GetKeyFromScancode(int scancode) {
    return scancode; // Direct mapping on Android stub
}

inline int SDL_SetHint(const char* name, const char* value) {
    return 0; // No-op on Android stub
}

// ========== CPU detection functions (for _math.cpp) ==========
// On Android ARM64, we only have NEON available (no x86 SIMD)

inline int SDL_HasRDTSC() {
    return 0; // x86 only
}

inline int SDL_HasMMX() {
    return 0; // x86 only
}

inline int SDL_Has3DNow() {
    return 0; // x86 only
}

inline int SDL_HasSSE() {
    return 0; // x86 only
}

inline int SDL_HasSSE2() {
    return 0; // x86 only
}

inline int SDL_HasSSE3() {
    return 0; // x86 only
}

inline int SDL_HasSSE41() {
    return 0; // x86 only
}

inline int SDL_HasSSE42() {
    return 0; // x86 only
}

inline int SDL_HasAVX() {
    return 0; // x86 only
}

inline int SDL_HasAVX2() {
    return 0; // x86 only
}

inline int SDL_HasAVX512F() {
    return 0; // x86 only
}

inline int SDL_HasAltiVec() {
    return 0; // PowerPC only
}

inline int SDL_HasARMSIMD() {
    return 0; // ARMv6 only
}

inline int SDL_HasNEON() {
    return 1; // ARM64 always has NEON
}

inline int SDL_HasLSX() {
    return 0; // LoongArch only
}

inline int SDL_HasLASX() {
    return 0; // LoongArch only
}

// ========== Timing functions (for _math.cpp) ==========
#include <time.h>
#include <sys/time.h>

inline Uint64 SDL_GetPerformanceFrequency() {
    return 1000000000ULL; // 1 GHz (nanosecond resolution)
}

inline Uint64 SDL_GetPerformanceCounter() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (Uint64)ts.tv_sec * 1000000000ULL + (Uint64)ts.tv_nsec;
}

inline Uint32 SDL_GetTicks() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (Uint32)((ts.tv_sec * 1000ULL) + (ts.tv_nsec / 1000000ULL));
}

// ========== SDL version check macro ==========
#define SDL_VERSION_ATLEAST(X, Y, Z) 0
// XR_CONTROLLER_AXIS_MAX is defined in xrEngine/xr_input.h as enum member, not macro

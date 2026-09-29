# S.T.A.L.K.E.R. OpenXRay Android (Native Quest 3 VR port)

Native Android port of OpenXRay engine for S.T.A.L.K.E.R.: Call of Pripyat using OpenGL ES 3.0 and Mali GPU acceleration.

## Features

- ✅ Native ARM64 (aarch64) build for Android
- ✅ OpenGL ES 3.0 rendering with Mali GPU support
- ✅ Android Bionic libc compatibility
- ✅ Full LuaJIT scripting support
- ✅ Direct access to Mali-G710 hardware acceleration
- 🔄 Automatic APK builds via GitHub Actions

## System Requirements

- **Android Version:** 7.0 (API 24) or higher
- **Architecture:** ARM64-v8a (64-bit ARM)
- **GPU:** Mali GPU with OpenGL ES 3.0+ support
- **RAM:** 4GB+ recommended
- **Storage:** 3GB+ for game data

## Building

### Automatic Build (GitHub Actions)

1. Fork this repository
2. Push to `main` or `dev` branch
3. GitHub Actions will automatically build the APK
4. Download the APK from Artifacts or Releases

### Manual Build (Termux)

```bash
# Install dependencies
pkg install openjdk-17 gradle

# Clone repository
git clone https://github.com/Standoff2bot/Stalker_android.git
cd Stalker_android

# Build APK
./gradlew assembleDebug
```

## Installation

1. Download the APK from [Releases](https://github.com/Standoff2bot/Stalker_android/releases)
2. Install on your Android device
3. Copy S.T.A.L.K.E.R.: Call of Pripyat game data to:
   ```
   /sdcard/Android/data/com.openxray.stalker/files/
   ```
4. Launch the app

## Game Data Setup

Required files structure:
```
/sdcard/Android/data/com.openxray.stalker/files/
├── fsgame.ltx
├── gamedata/
│   └── shaders/
│       └── gl/          # OpenGL shaders (required!)
├── resources/
│   ├── configs.db
│   ├── resources.db0-4
│   └── ...
├── levels/
├── localization/
└── patches/
```

**Important:** Original game uses DirectX shaders. You must use OpenGL shaders for Android build!

## Technical Details

### Architecture

- **Engine:** OpenXRay (X-Ray Engine 1.6)
- **Renderer:** OpenGL ES 3.0 (native Mali GPU)
- **Scripting:** LuaJIT 2.1.0-beta3 (Lua 5.1 API)
- **Audio:** OpenAL Soft
- **Physics:** ODE (Open Dynamics Engine)
- **Input:** SDL2 touch/gamepad support

### Performance

- **Mali-G710 MC10:** 60 FPS (native hardware acceleration)
- **Mali-G52+:** 30-45 FPS (OpenGL ES 3.0)
- **Software rendering:** Not recommended

### Differences from PC version

- OpenGL ES 3.0 instead of OpenGL 4.5/DirectX
- Touch controls and gamepad support
- Optimized shaders for mobile GPUs
- Android-specific file paths

## Development

### Project Structure

```
Stalker_android/
├── app/
│   ├── src/main/
│   │   ├── java/com/openxray/stalker/
│   │   │   └── MainActivity.java         # GLSurfaceView activity
│   │   ├── cpp/
│   │   │   ├── jni_bridge.cpp           # JNI native methods
│   │   │   ├── android_engine_wrapper.cpp
│   │   │   └── CMakeLists.txt           # NDK build config
│   │   ├── AndroidManifest.xml
│   │   └── assets/                       # Game data (optional)
│   └── build.gradle
├── .github/workflows/
│   └── build-apk.yml                     # CI/CD automation
└── README.md
```

### Key Components

1. **MainActivity.java** - Creates OpenGL ES 3.0 surface and handles lifecycle
2. **jni_bridge.cpp** - JNI layer between Java and C++ engine
3. **android_engine_wrapper.cpp** - OpenXRay engine initialization wrapper
4. **CMakeLists.txt** - Links OpenXRay libraries with Android NDK

## Original OpenXRay Source

This port uses the OpenXRay engine from:
- **Repository:** https://github.com/OpenXRay/xray-16
- **Branch:** xd_dev
- **Commit:** 29030f81b (Aug 14, 2026)

## Build Status

![Build APK](https://github.com/Standoff2bot/Stalker_android/workflows/Build%20OpenXRay%20Android%20APK/badge.svg)

## Credits

- **OpenXRay Team** - X-Ray Engine 1.6 development
- **GSC Game World** - Original S.T.A.L.K.E.R. series
- **Standoff2bot** - Android port and Mali GPU optimization

## License

OpenXRay engine is licensed under the Modified BSD / 3-clause BSD license.
See [OpenXRay LICENSE](https://github.com/OpenXRay/xray-16/blob/xd_dev/License.txt) for details.

S.T.A.L.K.E.R. game content is property of GSC Game World.

## Support

For issues and questions:
- Open an issue on [GitHub](https://github.com/Standoff2bot/Stalker_android/issues)
- Check OpenXRay wiki: https://github.com/OpenXRay/xray-16/wiki

---

**Good hunting, Stalker!** 🎮📱

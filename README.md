# S.T.A.L.K.E.R. OpenXRay Android (Quest 3 port groundwork)

Native ARM64 Android launch path for OpenXRay's S.T.A.L.K.E.R.: Call of Pripyat. The app is wired to the real engine and renderer and requests an OpenGL ES 3.2 surface.

This is the flat-screen Android baseline for the Quest 3 port. OpenXR, stereo eye rendering, headset tracking, and VR controller input are not integrated yet.

## Features

- Native ARM64 (aarch64) APK target
- OpenGL ES 3.2 renderer path
- Android Bionic libc compatibility
- Offline single-player main-menu startup path
- In-app import of user-owned Call of Pripyat data
- 🔄 Automatic APK builds via GitHub Actions

## System Requirements

- **Android Version:** 7.0 (API 24) or higher
- **Architecture:** ARM64-v8a (64-bit ARM)
- **GPU:** OpenGL ES 3.2 support required
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

1. Download and install the APK from [Releases](https://github.com/Standoff2bot/Stalker_android/releases)
2. Launch the app and choose your Call of Pripyat installation folder when prompted. You can also choose its `resources/` folder.
3. Wait for the app to copy the game data into its app-specific storage, then it will start the game.

## Game Data Setup

The APK supplies `fsgame.ltx` and the OpenXRay OpenGL shaders. The game data is not included. The selected Call of Pripyat folder should contain `resources/` with the `resources.db*` archives. The importer also copies `levels/`, `localization/`, `patches/`, and `gamedata/` when present, while preserving the Android shader files.

The app stores imported files under:

```
/sdcard/Android/data/com.openxray.stalker/files/
├── fsgame.ltx                 # Supplied by the APK
├── gamedata/shaders/gl/       # OpenXRay Android shaders, supplied by the APK
├── resources/                 # Imported Call of Pripyat archives
├── levels/                    # Imported when present
├── localization/              # Imported when present
└── patches/                   # Imported when present
```

The folder picker imports data recursively and can take several minutes for a multi-gigabyte installation. For large transfers, files can also be copied manually to the path above.

## Technical Details

### Architecture

- **Engine:** OpenXRay (X-Ray Engine 1.6)
- **Renderer:** OpenGL ES 3.2
- **Scripting:** LuaJIT 2.1.0-beta3 (Lua 5.1 API)
- **Audio:** Silent backend for the initial engine boot path
- **Physics:** ODE (Open Dynamics Engine)
- **Input:** Android touch and keyboard events feed the engine's SDL-compatible input queue

OpenXR stereo rendering, tracked VR input, and game audio remain to be implemented and tested on Quest 3 hardware.

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
│   │   │   ├── android_main.cpp         # OpenXRay startup and frame processing
│   │   │   ├── android_sound.cpp        # Silent boot-time audio backend
│   │   │   ├── SDL_stub.h               # Android SDL compatibility and input queue
│   │   │   └── CMakeLists.txt           # NDK build config
│   │   ├── AndroidManifest.xml
│   │   └── assets/                       # Game data (optional)
│   └── build.gradle
├── .github/workflows/
│   └── build-apk.yml                     # CI/CD automation
└── README.md
```

### Key Components

1. **MainActivity.java** - Creates OpenGL ES 3.2 surface, imports data, and handles lifecycle
2. **jni_bridge.cpp** - JNI layer between Java and C++ engine
3. **android_main.cpp** - OpenXRay engine initialization and frame processing
4. **CMakeLists.txt** - Links OpenXRay libraries with Android NDK

## Original OpenXRay Source

This port uses the OpenXRay engine from the checked-out submodule revision:
- **Repository:** https://github.com/OpenXRay/xray-16
- **Revision:** `97b691a986b03488d73058780f6855ed128062a3`

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

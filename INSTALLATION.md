# Game Data Installation Guide

## Where to Put Game Files

The APK contains the OpenXRay filesystem template and Android OpenGL shaders. You need to import the data from your own S.T.A.L.K.E.R.: Call of Pripyat installation.

### Installation Path

Place all game files in:
```
/sdcard/Android/data/com.openxray.stalker/files/
```

This directory will be automatically created when you first launch the app.

### Required File Structure

```
/sdcard/Android/data/com.openxray.stalker/files/
├── fsgame.ltx          # Supplied by the APK
├── gamedata/
│   ├── shaders/gl/     # OpenXRay Android shaders, supplied by the APK
│   └── configs/        # Imported from the game when present
├── resources/          # Packed game resources (REQUIRED)
│   ├── configs.db
│   ├── resources.db0
│   ├── resources.db1
│   ├── resources.db2
│   ├── resources.db3
│   └── resources.db4
├── levels/             # Game levels (REQUIRED)
│   ├── l01_escape/
│   ├── l02_garbage/
│   ├── ...
│   └── jupiter/
├── localization/       # Game text and dialogue (REQUIRED)
│   └── rus.xml         # or eng.xml for English
└── patches/            # Optional patches
```

## Step-by-Step Installation

### Option 1: In-App Import

1. Install and launch the APK.
2. When prompted, choose the Call of Pripyat installation folder or its `resources/` folder.
3. Wait while the app imports the `resources/` archives and any `levels/`, `localization/`, `patches/`, and `gamedata/` folders it finds.

Importing several gigabytes can take several minutes. Choosing the installation root lets the app copy the additional folders when they are present.

### Option 2: Using ADB

1. Connect your Android device to PC via USB
2. Enable USB debugging in Developer Options
3. Open terminal/command prompt on your PC
4. Navigate to your STALKER installation folder:
   ```bash
   cd "C:\Program Files\S.T.A.L.K.E.R. Call of Pripyat"
   ```
5. Push files to device:
   ```bash
   adb push fsgame.ltx /sdcard/Android/data/com.openxray.stalker/files/
   adb push resources /sdcard/Android/data/com.openxray.stalker/files/resources/
   adb push levels /sdcard/Android/data/com.openxray.stalker/files/levels/
   adb push localization /sdcard/Android/data/com.openxray.stalker/files/localization/
   ```

### Option 3: Manual Copy via File Manager

1. Copy your STALKER installation folder to your device (USB cable or cloud storage)
2. Use a file manager app (like "Files by Google" or "Total Commander")
3. Navigate to `/sdcard/Android/data/com.openxray.stalker/files/`
4. Copy the required folders there

### Option 4: Using Total Commander Plugin

1. Install Total Commander on Android
2. Connect to your PC via WiFi or USB
3. Copy game folders directly

## OpenGL Shaders

The APK installs the OpenXRay OpenGL shader set at `gamedata/shaders/gl/`. The importer skips this directory if the PC installation also contains shader files.

## Minimal Installation (Testing)

For initial testing, minimum required files (~2-3 GB):
1. `resources/` folder with the `resources.db*` archives
2. `levels/` and `localization/` folders from the installation, when present

## Troubleshooting

### App fails to start
- Check logcat: `adb logcat | grep OpenXRay`
- Verify `fsgame.ltx` exists
- Ensure the selected folder contained the `resources.db*` archives

### "Cannot find shader" error
- Check that the APK installed its files under `gamedata/shaders/gl/`
- Reinstall the APK if those files were removed

### "Cannot open file" errors
- Check file permissions
- Verify paths are correct (case-sensitive!)
- Ensure all .db archives are present in `resources/`

### OpenGL ES context error
- This build requires an OpenGL ES 3.2 context
- Confirm the device reports GLES 3.2 support

## File Sizes Reference

Approximate sizes for Call of Pripyat:
- `resources/*.db` - ~1.5 GB
- `levels/` - ~700 MB
- `gamedata/shaders/gl/` - installed from the APK
- `localization/` - ~10 MB

**Total required: ~2.3 GB minimum**

## Advanced: Custom Gamedata

If you want to use mods or custom gamedata:
1. Create `gamedata/` folder
2. Extract contents from `resources.db*` archives (if needed)
3. Mod files in `gamedata/` override packed resources
4. Keep `shaders/gl/` with OpenGL shaders (don't use DirectX ones!)

## Verification Commands

Check if files are correctly placed:
```bash
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/resources/
adb shell ls -lh /sdcard/Android/data/com.openxray.stalker/files/gamedata/shaders/gl/
```

## Getting Help

If you encounter issues:
1. Check app logs: `adb logcat -s OpenXRay:*`
2. Open an issue on GitHub: https://github.com/Standoff2bot/Stalker_android/issues
3. Include logcat output and device specifications

---

**Note:** You must own a legal copy of S.T.A.L.K.E.R.: Call of Pripyat to play this port. Game files are NOT included with the APK.

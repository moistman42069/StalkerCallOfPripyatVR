package com.openxray.stalker;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.database.Cursor;
import android.content.res.AssetManager;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.util.Log;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    private static final String TAG = "OpenXRay";
    private static final int REQUEST_CODE_PICK_FOLDER = 1001;
    private GLSurfaceView glSurfaceView;
    private String internalPath;
    private String externalPath;
    private boolean engineInitialized = false;
    private boolean startupErrorShown = false;

    static {
        System.loadLibrary("xray-engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Internal storage for app data (configs, logs)
        internalPath = getFilesDir().getAbsolutePath();

        // External storage for game data (resources, levels, gamedata)
        // User should place game files in: /sdcard/Android/data/com.openxray.stalker/files/
        File externalFilesDir = getExternalFilesDir(null);
        externalPath = externalFilesDir != null ?
            externalFilesDir.getAbsolutePath() :
            Environment.getExternalStorageDirectory().getAbsolutePath() + "/stalker";

        Log.i(TAG, "Internal path: " + internalPath);
        Log.i(TAG, "External path: " + externalPath);

        // Create external directory if it doesn't exist
        File externalDir = new File(externalPath);
        if (!externalDir.exists()) {
            externalDir.mkdirs();
            Log.i(TAG, "Created external directory: " + externalPath);
        }

        // OpenXRay's redistributable GLES shader set and filesystem template
        // are packaged in the APK. The copyrighted Call of Pripyat data stays
        // user-supplied.
        try {
            copyAssetIfMissing("fsgame.ltx", new File(externalDir, "fsgame.ltx"));
            copyAssetDirectory("gamedata/shaders/gl", new File(externalDir, "gamedata/shaders/gl"));
        } catch (IOException e) {
            Log.e(TAG, "Could not install OpenXRay bootstrap data", e);
            Toast.makeText(this, "Could not prepare OpenXRay files: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }

        // Check if game files exist
        if (!checkGameFiles()) {
            showGameFilesDialog();
            return;
        }

        initializeEngine();
    }

    private boolean checkGameFiles() {
        // The engine supplies fsgame.ltx and OpenGL shaders. Require the user's
        // actual CoP resource archives so a stray folder cannot trigger a boot.
        File resourcesDir = new File(externalPath, "resources");
        File fsgameLtx = new File(externalPath, "fsgame.ltx");
        File shadersDir = new File(externalPath, "gamedata/shaders/gl");
        boolean hasResourceArchive = false;
        String[] resourceFiles = resourcesDir.list();
        if (resourceFiles != null) {
            for (String name : resourceFiles) {
                if (name.startsWith("resources.db") && !name.endsWith(".importing")) {
                    hasResourceArchive = true;
                    break;
                }
            }
        }

        Log.i(TAG, "Checking game files:");
        Log.i(TAG, "  resource archive: " + hasResourceArchive);
        Log.i(TAG, "  shaders: " + shadersDir.isDirectory());
        Log.i(TAG, "  fsgame.ltx: " + fsgameLtx.isFile());

        return hasResourceArchive && shadersDir.isDirectory() && fsgameLtx.isFile();
    }

    private void copyAssetIfMissing(String assetPath, File destination) throws IOException {
        if (destination.isFile() && destination.length() > 0) {
            return;
        }
        File parent = destination.getParentFile();
        if (parent != null && !parent.exists() && !parent.mkdirs()) {
            throw new IOException("Cannot create " + parent);
        }
        try (InputStream input = getAssets().open(assetPath);
             FileOutputStream output = new FileOutputStream(destination)) {
            byte[] buffer = new byte[32768];
            int count;
            while ((count = input.read(buffer)) != -1) {
                output.write(buffer, 0, count);
            }
        }
    }

    private void copyAssetDirectory(String assetPath, File destination) throws IOException {
        AssetManager assets = getAssets();
        String[] children = assets.list(assetPath);
        if (children == null || children.length == 0) {
            copyAssetIfMissing(assetPath, destination);
            return;
        }
        if (!destination.exists() && !destination.mkdirs()) {
            throw new IOException("Cannot create " + destination);
        }
        for (String child : children) {
            copyAssetDirectory(assetPath + "/" + child, new File(destination, child));
        }
    }

    private void showGameFilesDialog() {
        new AlertDialog.Builder(this)
            .setTitle("Game Files Not Found")
            .setMessage("OpenXRay requires S.T.A.L.K.E.R. game files.\n\n" +
                "Choose the Call of Pripyat installation folder, or its resources/ folder. The app will " +
                "import the resources.db archives and any levels/, localization/, patches/, and gamedata/ " +
                "folders it finds. Importing several gigabytes can take a while.\n\n" +
                "The APK supplies the OpenXRay filesystem template and OpenGL shaders.\n\n" +
                "Copy them to:\n" +
                externalPath + "\n\n" +
                "You can access this path using:\n" +
                "• File manager app\n" +
                "• USB connection (MTP)\n" +
                "• adb push command")
            .setPositiveButton("Choose Folder", (dialog, which) -> {
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
                startActivityForResult(intent, REQUEST_CODE_PICK_FOLDER);
            })
            .setNegativeButton("Exit", (dialog, which) -> finish())
            .setNeutralButton("Retry", (dialog, which) -> {
                if (checkGameFiles()) {
                    initializeEngine();
                } else {
                    showGameFilesDialog();
                }
            })
            .setCancelable(false)
            .show();
    }

    private void initializeEngine() {
        // Store paths before starting GLSurfaceView's GL thread. A fast device
        // can deliver the first surface callback as soon as the renderer is set.
        try {
            nativeInit(internalPath, externalPath);
        } catch (Exception e) {
            Log.e(TAG, "Failed to initialize engine", e);
            new AlertDialog.Builder(this)
                .setTitle("Initialization Error")
                .setMessage("Failed to initialize OpenXRay engine:\n\n" + e.getMessage() +
                    "\n\nMake sure all game files are copied correctly.")
                .setPositiveButton("OK", (dialog, which) -> finish())
                .show();
            return;
        }

        glSurfaceView = new GameSurfaceView(this);
        glSurfaceView.setEGLContextFactory(new GLSurfaceView.EGLContextFactory() {
            @Override
            public javax.microedition.khronos.egl.EGLContext createContext(
                    javax.microedition.khronos.egl.EGL10 egl,
                    javax.microedition.khronos.egl.EGLDisplay display,
                    javax.microedition.khronos.egl.EGLConfig config) {
                final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
                final int EGL_CONTEXT_MINOR_VERSION_KHR = 0x30FB;
                int[] attributes = {
                    EGL_CONTEXT_CLIENT_VERSION, 3,
                    EGL_CONTEXT_MINOR_VERSION_KHR, 2,
                    javax.microedition.khronos.egl.EGL10.EGL_NONE
                };
                return egl.eglCreateContext(display, config,
                    javax.microedition.khronos.egl.EGL10.EGL_NO_CONTEXT, attributes);
            }

            @Override
            public void destroyContext(javax.microedition.khronos.egl.EGL10 egl,
                    javax.microedition.khronos.egl.EGLDisplay display,
                    javax.microedition.khronos.egl.EGLContext context) {
                egl.eglDestroyContext(display, context);
            }
        });
        glSurfaceView.setEGLConfigChooser((egl, display) -> {
            final int EGL_RENDERABLE_TYPE = 0x3040;
            final int EGL_OPENGL_ES3_BIT_KHR = 0x0040;
            int[] attributes = {
                javax.microedition.khronos.egl.EGL10.EGL_RED_SIZE, 8,
                javax.microedition.khronos.egl.EGL10.EGL_GREEN_SIZE, 8,
                javax.microedition.khronos.egl.EGL10.EGL_BLUE_SIZE, 8,
                javax.microedition.khronos.egl.EGL10.EGL_ALPHA_SIZE, 8,
                javax.microedition.khronos.egl.EGL10.EGL_DEPTH_SIZE, 24,
                javax.microedition.khronos.egl.EGL10.EGL_STENCIL_SIZE, 8,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                javax.microedition.khronos.egl.EGL10.EGL_NONE
            };
            javax.microedition.khronos.egl.EGLConfig[] configs = new javax.microedition.khronos.egl.EGLConfig[1];
            int[] configCount = new int[1];
            if (!egl.eglChooseConfig(display, attributes, configs, 1, configCount) || configCount[0] == 0) {
                throw new IllegalArgumentException("OpenGL ES 3.x EGL configuration is unavailable");
            }
            return configs[0];
        });
        glSurfaceView.setPreserveEGLContextOnPause(true);
        glSurfaceView.setRenderer(new XRayRenderer());
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        glSurfaceView.setFocusable(true);
        glSurfaceView.setFocusableInTouchMode(true);

        setContentView(glSurfaceView);
        glSurfaceView.requestFocus();
    }

    private final class GameSurfaceView extends GLSurfaceView {
        GameSurfaceView(Activity activity) {
            super(activity);
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            if (engineInitialized) {
                int action = event.getActionMasked();
                int x = Math.round(event.getX());
                int y = Math.round(event.getY());
                if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_MOVE ||
                        action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
                    nativeSendTouch(action, x, y);
                }
            }
            return true;
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (engineInitialized) {
            int scancode = toSdlScancode(event.getKeyCode());
            if (scancode != 0) {
                nativeSendKey(scancode, event.getAction() == KeyEvent.ACTION_DOWN,
                    event.getRepeatCount() > 0);
                return true;
            }
        }
        return super.dispatchKeyEvent(event);
    }

    private int toSdlScancode(int keyCode) {
        if (keyCode >= KeyEvent.KEYCODE_A && keyCode <= KeyEvent.KEYCODE_Z) {
            return 4 + keyCode - KeyEvent.KEYCODE_A;
        }
        if (keyCode >= KeyEvent.KEYCODE_1 && keyCode <= KeyEvent.KEYCODE_9) {
            return 30 + keyCode - KeyEvent.KEYCODE_1;
        }
        if (keyCode >= KeyEvent.KEYCODE_F1 && keyCode <= KeyEvent.KEYCODE_F12) {
            return 58 + keyCode - KeyEvent.KEYCODE_F1;
        }
        switch (keyCode) {
            case KeyEvent.KEYCODE_0: return 39;
            case KeyEvent.KEYCODE_DPAD_RIGHT: return 79;
            case KeyEvent.KEYCODE_DPAD_LEFT: return 80;
            case KeyEvent.KEYCODE_DPAD_DOWN: return 81;
            case KeyEvent.KEYCODE_DPAD_UP: return 82;
            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_DPAD_CENTER:
            case KeyEvent.KEYCODE_BUTTON_A: return 40;
            case KeyEvent.KEYCODE_ESCAPE:
            case KeyEvent.KEYCODE_BACK:
            case KeyEvent.KEYCODE_BUTTON_B: return 41;
            case KeyEvent.KEYCODE_DEL: return 42;
            case KeyEvent.KEYCODE_TAB: return 43;
            case KeyEvent.KEYCODE_SPACE:
            case KeyEvent.KEYCODE_BUTTON_X: return 44;
            case KeyEvent.KEYCODE_SHIFT_LEFT: return 225;
            case KeyEvent.KEYCODE_SHIFT_RIGHT: return 229;
            case KeyEvent.KEYCODE_ALT_LEFT: return 226;
            case KeyEvent.KEYCODE_ALT_RIGHT: return 230;
            case KeyEvent.KEYCODE_CTRL_LEFT: return 224;
            case KeyEvent.KEYCODE_CTRL_RIGHT: return 228;
            case KeyEvent.KEYCODE_BUTTON_Y: return 43;
            case KeyEvent.KEYCODE_MINUS: return 45;
            case KeyEvent.KEYCODE_EQUALS: return 46;
            case KeyEvent.KEYCODE_COMMA: return 54;
            case KeyEvent.KEYCODE_PERIOD: return 55;
            case KeyEvent.KEYCODE_SLASH: return 56;
            default: return 0;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_PICK_FOLDER && resultCode == RESULT_OK) {
            if (data != null) {
                Uri treeUri = data.getData();
                int takeFlags = data.getFlags() & Intent.FLAG_GRANT_READ_URI_PERMISSION;
                try {
                    getContentResolver().takePersistableUriPermission(treeUri, takeFlags);
                } catch (SecurityException e) {
                    Log.w(TAG, "Could not persist selected folder access", e);
                }

                ProgressDialog progress = new ProgressDialog(this);
                progress.setMessage("Importing Call of Pripyat files…");
                progress.setCancelable(false);
                progress.show();
                new Thread(() -> {
                    String error = null;
                    try {
                        importGameFiles(treeUri);
                    } catch (Exception e) {
                        Log.e(TAG, "Game data import failed", e);
                        error = e.getMessage() == null ? e.toString() : e.getMessage();
                    }
                    final String importError = error;
                    runOnUiThread(() -> {
                        progress.dismiss();
                        if (importError != null) {
                            new AlertDialog.Builder(MainActivity.this)
                                .setTitle("Import Failed")
                                .setMessage(importError + "\n\nChoose the Call of Pripyat install folder, or its resources folder.")
                                .setPositiveButton("OK", (d, w) -> showGameFilesDialog())
                                .show();
                        } else if (checkGameFiles()) {
                            initializeEngine();
                        } else {
                            new AlertDialog.Builder(MainActivity.this)
                                .setTitle("Game Archives Not Found")
                                .setMessage("The selected folder did not contain resources.db archives. Choose the Call of Pripyat install folder or its resources folder.")
                                .setPositiveButton("Choose Again", (d, w) -> showGameFilesDialog())
                                .setNegativeButton("Exit", (d, w) -> finish())
                                .show();
                        }
                    });
                }, "game-data-import").start();
            }
        }
    }

    private void importGameFiles(Uri treeUri) throws IOException {
        String rootId = DocumentsContract.getTreeDocumentId(treeUri);
        Uri rootDocument = DocumentsContract.buildDocumentUriUsingTree(treeUri, rootId);
        String rootName = queryDocumentName(rootDocument);
        File externalDir = new File(externalPath);
        boolean importedResources = false;

        if ("resources".equalsIgnoreCase(rootName)) {
            copyDocumentDirectory(treeUri, rootId, new File(externalDir, "resources"), "resources");
            importedResources = true;
        } else {
            for (DocumentEntry entry : listDocumentChildren(treeUri, rootId)) {
                String name = entry.name.toLowerCase(Locale.ROOT);
                if (!entry.isDirectory || !(name.equals("resources") || name.equals("levels") ||
                        name.equals("localization") || name.equals("patches") || name.equals("gamedata"))) {
                    continue;
                }
                copyDocumentDirectory(treeUri, entry.id, new File(externalDir, entry.name), entry.name);
                if (name.equals("resources")) {
                    importedResources = true;
                }
            }
        }

        if (!importedResources) {
            throw new IOException("No resources/ folder was found in the selected location.");
        }
    }

    private static final class DocumentEntry {
        final String id;
        final String name;
        final boolean isDirectory;

        DocumentEntry(String id, String name, boolean isDirectory) {
            this.id = id;
            this.name = name;
            this.isDirectory = isDirectory;
        }
    }

    private String queryDocumentName(Uri documentUri) throws IOException {
        try (Cursor cursor = getContentResolver().query(documentUri,
                new String[]{DocumentsContract.Document.COLUMN_DISPLAY_NAME}, null, null, null)) {
            if (cursor != null && cursor.moveToFirst()) {
                return cursor.getString(0);
            }
        }
        throw new IOException("Could not read the selected folder name.");
    }

    private java.util.List<DocumentEntry> listDocumentChildren(Uri treeUri, String parentId) throws IOException {
        Uri childrenUri = DocumentsContract.buildChildDocumentsUriUsingTree(treeUri, parentId);
        java.util.ArrayList<DocumentEntry> entries = new java.util.ArrayList<>();
        try (Cursor cursor = getContentResolver().query(childrenUri,
                new String[]{DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                    DocumentsContract.Document.COLUMN_DISPLAY_NAME,
                    DocumentsContract.Document.COLUMN_MIME_TYPE}, null, null, null)) {
            if (cursor == null) {
                throw new IOException("The selected folder could not be read.");
            }
            while (cursor.moveToNext()) {
                String id = cursor.getString(0);
                String name = cursor.getString(1);
                boolean isDirectory = DocumentsContract.Document.MIME_TYPE_DIR.equals(cursor.getString(2));
                entries.add(new DocumentEntry(id, name, isDirectory));
            }
        }
        return entries;
    }

    private void copyDocumentDirectory(Uri treeUri, String documentId, File destination, String relativePath)
            throws IOException {
        if (!destination.exists() && !destination.mkdirs()) {
            throw new IOException("Could not create " + destination);
        }
        for (DocumentEntry child : listDocumentChildren(treeUri, documentId)) {
            if (child.name == null || child.name.isEmpty() || child.name.equals(".") || child.name.equals("..") ||
                    child.name.contains("/") || child.name.contains("\\")) {
                throw new IOException("The selected folder contains an invalid file name.");
            }
            File childDestination = new File(destination, child.name);
            String childRelativePath = relativePath + "/" + child.name;
            // Preserve the Android GLES shaders shipped with the APK when the
            // selected PC install also contains its own shader directory.
            if ("gamedata/shaders/gl".equalsIgnoreCase(childRelativePath.replace('\\', '/')) && child.isDirectory) {
                continue;
            }
            if (child.isDirectory) {
                copyDocumentDirectory(treeUri, child.id, childDestination, childRelativePath);
            } else {
                Uri fileUri = DocumentsContract.buildDocumentUriUsingTree(treeUri, child.id);
                File partialFile = new File(childDestination.getPath() + ".importing");
                try {
                    try (InputStream input = getContentResolver().openInputStream(fileUri);
                         FileOutputStream output = new FileOutputStream(partialFile)) {
                        if (input == null) {
                            throw new IOException("Could not open " + child.name);
                        }
                        byte[] buffer = new byte[65536];
                        int count;
                        while ((count = input.read(buffer)) != -1) {
                            output.write(buffer, 0, count);
                        }
                    }
                    if (childDestination.exists() && !childDestination.delete()) {
                        throw new IOException("Could not replace " + child.name);
                    }
                    if (!partialFile.renameTo(childDestination)) {
                        throw new IOException("Could not finish importing " + child.name);
                    }
                } catch (IOException e) {
                    partialFile.delete();
                    throw e;
                }
            }
        }
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        if (glSurfaceView != null) {
            if (engineInitialized) {
                glSurfaceView.queueEvent(() -> nativePause());
            }
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (glSurfaceView != null) {
            glSurfaceView.onResume();
            if (engineInitialized) {
                glSurfaceView.queueEvent(() -> nativeResume());
            }
        }
    }

    @Override
    protected void onDestroy() {
        if (engineInitialized && glSurfaceView != null) {
            glSurfaceView.queueEvent(() -> nativeDestroy());
        }
        super.onDestroy();
    }

    private class XRayRenderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            nativeSurfaceCreated();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            int result = nativeSurfaceChanged(width, height);
            if (result == 0) {
                if (!engineInitialized) {
                    runOnUiThread(() -> {
                        engineInitialized = true;
                        Toast.makeText(MainActivity.this, "Call of Pripyat started", Toast.LENGTH_SHORT).show();
                    });
                }
            } else if (!startupErrorShown) {
                startupErrorShown = true;
                String error = nativeGetStartupError();
                runOnUiThread(() -> new AlertDialog.Builder(MainActivity.this)
                    .setTitle("Game Startup Failed")
                    .setMessage(error.isEmpty() ? "OpenXRay could not start. Check the game data and logcat." : error)
                    .setPositiveButton("Exit", (dialog, which) -> finish())
                    .show());
            }
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            nativeDrawFrame();
        }
    }

    // Native methods
    private static native void nativeInit(String internalPath, String externalPath);
    private static native void nativeSurfaceCreated();
    private static native int nativeSurfaceChanged(int width, int height);
    private static native String nativeGetStartupError();
    private static native void nativeSendTouch(int action, int x, int y);
    private static native void nativeSendKey(int scancode, boolean down, boolean repeat);
    private static native void nativeDrawFrame();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeDestroy();
}

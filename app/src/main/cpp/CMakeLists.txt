cmake_minimum_required(VERSION 3.22)
project(xray-android)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OpenXRay source root
set(XRAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/src)
set(XRAY_EXTERNALS ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/Externals)
set(XRAY_SDK ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/sdk)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16/cmake/utils.cmake)

# Keep OpenXRay's offline main menu on Android while omitting its desktop-only
# GameSpy login code. The patch is applied to the checked-out submodule only.
file(READ ${XRAY_SRC}/xrGame/MainMenu.cpp XR_MAIN_MENU_SOURCE)
string(FIND "${XR_MAIN_MENU_SOURCE}" "ANDROID_OFFLINE_MAIN_MENU" XR_MAIN_MENU_PATCHED)
file(READ ${XRAY_SRC}/Layers/xrRenderGL/glHW.cpp XR_GL_HW_SOURCE)
string(FIND "${XR_GL_HW_SOURCE}" "gladLoadGLES2" XR_GLES_RENDERER_PATCHED)
if(XR_MAIN_MENU_PATCHED EQUAL -1 OR XR_GLES_RENDERER_PATCHED EQUAL -1)
    execute_process(
        COMMAND patch -p1 --forward -i ${CMAKE_CURRENT_SOURCE_DIR}/patches/MainMenu.android.patch
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../xray-16
        RESULT_VARIABLE XR_MAIN_MENU_PATCH_RESULT
        OUTPUT_VARIABLE XR_MAIN_MENU_PATCH_OUTPUT
        ERROR_VARIABLE XR_MAIN_MENU_PATCH_ERROR
    )
    if(NOT XR_MAIN_MENU_PATCH_RESULT EQUAL 0)
        message(FATAL_ERROR "Could not apply Android main-menu patch: ${XR_MAIN_MENU_PATCH_OUTPUT}${XR_MAIN_MENU_PATCH_ERROR}")
    endif()
endif()

# Platform defines
add_compile_definitions(
    ANDROID
    LINUX
    _LINUX
    XR_PLATFORM_LINUX
    XRAY_STATIC_BUILD
    dSINGLE  # ODE physics engine: use single-precision floats
    USE_OGL  # Use OpenXRay's OpenGL ES renderer path
    R_GL=0
    R_R1=1
    R_R2=2
    R_R3=3
    R_R4=4
    RENDER=R_GL
    RENDER_NAMESPACE=render_gl
)

# Build configuration (needed for Platform.hpp line 108 check)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
else()
    add_compile_definitions(NDEBUG)
endif()

# Android compatibility layer (replaces SDL2 dependencies)
add_compile_definitions(
    SDL_stdinc_h_=1
)

# Force include our SDL stub before any SDL.h includes
# Add -ferror-limit=0 to see ALL errors at once (not just first 20)
# Add -w to suppress all warnings (save memory and compilation time)
add_compile_options(
    -include ${CMAKE_CURRENT_SOURCE_DIR}/SDL_stub.h
    -ferror-limit=0
    -w
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${XRAY_SRC}
    ${XRAY_SRC}/xrCore
    ${XRAY_SRC}/xrEngine
    ${XRAY_SRC}/xrCDB
    ${XRAY_SRC}/xrSound
    ${XRAY_SRC}/xrGame
    ${XRAY_SRC}/xrServerEntities
    ${XRAY_SRC}/xrNetServer
    ${XRAY_SRC}/xrScriptEngine
    ${XRAY_SRC}/xrPhysics
    ${XRAY_SRC}/xrParticles
    ${XRAY_SRC}/xrUICore
    ${XRAY_SRC}/xrAICore
    ${XRAY_SRC}/Layers
    ${XRAY_SRC}/Common
    ${XRAY_SRC}/Include
    ${XRAY_EXTERNALS}/tracy/public
    ${XRAY_EXTERNALS}/LuaJIT/src
    ${XRAY_EXTERNALS}/luabind
    ${XRAY_EXTERNALS}/sse2neon
    ${XRAY_EXTERNALS}/imgui
    ${XRAY_EXTERNALS}/gli
    ${XRAY_EXTERNALS}/ode/include
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/include
    ${XRAY_EXTERNALS}
    ${XRAY_SDK}/include
)

# Find Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)
find_library(dl-lib dl)

# xrCore - critical base library
file(GLOB_RECURSE XRCORE_SOURCES
    ${XRAY_SRC}/xrCore/*.cpp
    ${XRAY_SRC}/xrCommon/*.cpp
)

# Exclude Windows-specific and editor-only files
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*_win32.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*Windows.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*dx.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*Editor.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*BoneEditor.*")
# StackTrace - keep it (needed by linker)
# list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*StackTrace.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*ModuleLookup.*")
# ThreadUtil - keep it (needed for TaskManager)
# list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*ThreadUtil.*")
list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*mailSlot.*")
# TaskManager - keep it (needed by linker)
# list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*/Threading/TaskManager.*")
# _math.cpp - keep it (contains _vector3::magnitude)
# list(FILTER XRCORE_SOURCES EXCLUDE REGEX ".*/_math\\.cpp")

# Add LZO compression library sources
file(GLOB LZO_SOURCES
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo_init.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo_util.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo1x_1.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo1x_9x.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo1x_d1.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo1x_d2.c
    ${XRAY_EXTERNALS}/lzo/lzo-2.10/src/lzo1x_d3.c
)

# Add xrMiscMath utility sources (template instantiations for _vector3 and Fmatrix)
set(XRMISCMATH_SOURCES
    ${XRAY_SRC}/utils/xrMiscMath/vector.cpp
    ${XRAY_SRC}/utils/xrMiscMath/matrix.cpp
)

add_library(xrCore STATIC ${XRCORE_SOURCES} ${LZO_SOURCES} ${XRMISCMATH_SOURCES})
target_compile_options(xrCore PRIVATE
    -DXRCORE_EXPORTS
    -Wno-deprecated-declarations
    -fno-strict-aliasing
    -fexceptions
    -ferror-limit=0
)

# xrCDB - collision detection
file(GLOB_RECURSE XRCDB_SOURCES ${XRAY_SRC}/xrCDB/*.cpp)
add_library(xrCDB STATIC ${XRCDB_SOURCES})
target_link_libraries(xrCDB PRIVATE xrCore)

# xrScriptEngine - Lua integration
file(GLOB_RECURSE XRSCRIPT_SOURCES ${XRAY_SRC}/xrScriptEngine/*.cpp)
add_library(xrScriptEngine STATIC ${XRSCRIPT_SOURCES})
target_link_libraries(xrScriptEngine PRIVATE xrCore)

# xrSound temporarily disabled (ENGINE_API dependency issue)
# file(GLOB_RECURSE XRSOUND_SOURCES ${XRAY_SRC}/xrSound/*.cpp)
# list(FILTER XRSOUND_SOURCES EXCLUDE REGEX ".*OpenALDeviceList.*")
# add_library(xrSound STATIC ${XRSOUND_SOURCES})
# target_link_libraries(xrSound PRIVATE xrCore)

# xrEngine - main engine
file(GLOB_RECURSE XRENGINE_SOURCES ${XRAY_SRC}/xrEngine/*.cpp)
list(FILTER XRENGINE_SOURCES EXCLUDE REGEX ".*xrRender.*")
list(FILTER XRENGINE_SOURCES EXCLUDE REGEX ".*Text_Console.*")
list(FILTER XRENGINE_SOURCES EXCLUDE REGEX ".*tntQAVI.*")
add_library(xrEngine STATIC ${XRENGINE_SOURCES})
# Force-include SDL_stub.h for xrEngine (device.h, xr_input.h use SDL types)
# Use target_compile_options instead of COMPILE_FLAGS to avoid overwriting global flags
target_compile_options(xrEngine PRIVATE
    -include ${CMAKE_CURRENT_SOURCE_DIR}/SDL_stub.h
)
target_link_libraries(xrEngine PRIVATE
    xrCore
    xrCDB
)

# The old Android shim compiled shared renderer files with a placeholder
# CRenderImplementation. That could never render a game. Build OpenXRay's real
# OpenGL renderer, which provides the renderer module and R2 implementation.
add_subdirectory(${XRAY_SRC}/Layers/xrAPI ${CMAKE_CURRENT_BINARY_DIR}/xrAPI)

# This Android target already compiles xrMiscMath's vector/matrix instantiations
# into xrCore, so expose the upstream dependency name without compiling them twice.
add_library(xrMiscMath INTERFACE)

# The initial Android boot path has audio disabled. Keep the engine interfaces
# available through the no-output backend in android_sound.cpp.
add_library(xrSound INTERFACE)

add_library(xrMaterialSystem STATIC
    ${XRAY_SRC}/xrMaterialSystem/GameMtlLib.cpp
    ${XRAY_SRC}/xrMaterialSystem/GameMtlLib_Engine.cpp
)
target_include_directories(xrMaterialSystem PRIVATE ${XRAY_SRC}/xrMaterialSystem)
target_compile_definitions(xrMaterialSystem PRIVATE XRMTL_LIB_EXPORTS)
target_link_libraries(xrMaterialSystem PRIVATE xrMiscMath xrAPI xrCore)

set(IMGUI_DIR ${XRAY_EXTERNALS}/imgui)
add_library(xrImGui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(xrImGui PUBLIC ${IMGUI_DIR})
target_compile_definitions(xrImGui PUBLIC
    IMGUI_DISABLE_OBSOLETE_KEYIO
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    IMGUI_DEFINE_MATH_OPERATORS
    IMGUI_IMPL_OPENGL_ES3
)

add_subdirectory(${XRAY_SRC}/xrAICore ${CMAKE_CURRENT_BINARY_DIR}/xrAICore)
add_subdirectory(${XRAY_SRC}/xrUICore ${CMAKE_CURRENT_BINARY_DIR}/xrUICore)

# xrParticles - particle system
file(GLOB_RECURSE XRPARTICLES_SOURCES ${XRAY_SRC}/xrParticles/*.cpp)
list(FILTER XRPARTICLES_SOURCES EXCLUDE REGEX ".*Editor.*")
list(FILTER XRPARTICLES_SOURCES EXCLUDE REGEX ".*_test.*")
add_library(xrParticles STATIC ${XRPARTICLES_SOURCES})
target_compile_definitions(xrParticles PRIVATE ECORE_API=)
target_link_libraries(xrParticles PRIVATE
    xrCore
    xrEngine
)

# xrPhysics - ODE physics engine wrapper
file(GLOB_RECURSE XRPHYSICS_SOURCES ${XRAY_SRC}/xrPhysics/*.cpp)
list(FILTER XRPHYSICS_SOURCES EXCLUDE REGEX ".*Editor.*")
list(FILTER XRPHYSICS_SOURCES EXCLUDE REGEX ".*_test.*")
list(FILTER XRPHYSICS_SOURCES EXCLUDE REGEX ".*/tri-colliderknoopc/dcTriListCollider\\.cpp$")
add_library(xrPhysics STATIC ${XRPHYSICS_SOURCES})
target_compile_definitions(xrPhysics PRIVATE ECORE_API= XRPHYSICS_EXPORTS PURE_DYNAMIC_CAST)
target_precompile_headers(xrPhysics PRIVATE ${XRAY_SRC}/xrPhysics/StdAfx.h)
target_link_libraries(xrPhysics PRIVATE
    xrCore
    xrEngine
    xrCDB
)

# Add the renderer after its particle library exists so CMake records the
# dependency as a target instead of a bare linker name.
add_subdirectory(${XRAY_SRC}/Layers/xrRenderPC_GL ${CMAKE_CURRENT_BINARY_DIR}/xrRenderPC_GL)

# xrServerEntities
file(GLOB_RECURSE XRSERVER_SOURCES ${XRAY_SRC}/xrServerEntities/*.cpp)
# Exclude editor-only files (use Windows types like LPCSTR, BOOL without proper includes)
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*PropertiesListTypes.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*ItemListTypes.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*xrEProps.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*PropertiesListHelper.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*script_properties_list_helper.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*script_rtoken_list.*")
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*script_ini_file.*")
# Exclude xml_str_id_loader (has variadic function issues with xr_string on Android)
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*xml_str_id_loader.*")
# Exclude ALL script binding files (use smart_cast.h template metaprogramming incompatible with Release optimizations)
list(FILTER XRSERVER_SOURCES EXCLUDE REGEX ".*script_.*_script\\.cpp$")
add_library(xrServerEntities STATIC ${XRSERVER_SOURCES})
target_link_libraries(xrServerEntities PRIVATE xrCore)

# xrGame - game logic
file(GLOB_RECURSE XRGAME_SOURCES ${XRAY_SRC}/xrGame/*.cpp)
# Exclude DynamicHeightMap (uses old RAPID collision API, not used in game)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*DynamicHeightMap.*")
# Exclude GameSpy multiplayer (not needed for single-player Android port)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*GameSpy.*")
# Exclude network files (depend on GameSpy, not needed for single-player)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*_network_.*")
# Exclude Actor network code
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*Actor_Network.*")
# Exclude ALL multiplayer files (*_mp_* pattern catches actor_mp_server, etc.)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*_mp_.*")
# Exclude multiplayer-related files
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(account_manager|login_manager|ServerList|xrServer_C).*")
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(game_cl_mp|game_sv_mp|game_sv_base|console_commands_mp).*")
# Exclude player profiles and account system (depend on GameSpy/multiplayer)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(player_account|profile_store|profile_data_types).*")
# Exclude Level GameSpy and network functions
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(Level_GameSpy|xrGameSpy|Level_network).*")
# Exclude ALL multiplayer game modes (deathmatch, teamdeathmatch, artefacthunt, capture_the_artefact)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(deathmatch|teamdeathmatch|artefacthunt|capture_the_artefact).*")
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*(MPPlayersBag).*")
# Keep MainMenu and the game UI: CGamePersistent creates them during startup.
# Exclude MathUtils (test/debug code with API incompatibilities)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*MathUtils.*")
# The obsolete fog-of-war UI is commented out in OpenXRay's own game target and
# includes a removed `ui/UIWindow.h` path.
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*LevelFogOfWar.*")
# These legacy, optional, or platform-specific sources are commented out in
# OpenXRay's own xrGame target. Some depend on headers removed from this build.
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*/(AnselManager|stalker_animation_offsets|UIFrameRect|UIGame_custom_script|CExtraContentFilter|UIFrags2|UIFrags|UIFrameLine|UIListItemAdv|UITextBanner)\\.cpp$")
# Exclude ObjectDump (debug-only code with CObject dependencies, uses Fbox instead of Fbox3)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*ObjectDump.*")
# Exclude test files (unit tests, not needed for game build)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*_test\\.cpp$")
# Exclude rats AI (legacy/old AI code with missing dependencies)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*/rats/.*")
# Exclude ALife offline simulation (47 files, complex system with XRGAME_EXPORTS dependencies)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*alife_.*")
# Exclude evaluation functions (ef_* files - depend on ALife system)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*ef_.*")
# Exclude character_info and specific_character (depend on xml_str_id_loader.h which has variadic function issues)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*character_info.*")
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*specific_character.*")
# Exclude xrgame_dll_detach (includes character_info.h which includes xml_str_id_loader.h)
list(FILTER XRGAME_SOURCES EXCLUDE REGEX ".*xrgame_dll_detach.*")
add_library(xrGame STATIC ${XRGAME_SOURCES})
target_link_libraries(xrGame PRIVATE
    xrCore
    xrEngine
    xrCDB
    xrServerEntities
    xrScriptEngine
    xrMaterialSystem
    xrMiscMath
    xrImGui
    xrAPI
    xrAICore
    xrUICore
    xrSound
    xrParticles
    xrPhysics
)

# Main JNI wrapper library
add_library(xray-engine SHARED
    jni_bridge.cpp
    android_main.cpp
    android_sound.cpp
)

target_link_libraries(xray-engine
    PRIVATE
    xrCore
    xrCDB
    xrEngine
    xrRender_GL
    xrParticles
    xrPhysics
    xrScriptEngine
    xrGame
    -Wl,--start-group
    xrServerEntities
    xrGame
    -Wl,--end-group
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
    ${dl-lib}
)

target_compile_options(xray-engine PRIVATE
    -Wno-deprecated-declarations
    -fno-strict-aliasing
    -fexceptions
)

#pragma once

// Precompiled header for xrRender module
// Force-included for all xrRender source files to provide missing types

// SDL stub (must be BEFORE any xrEngine includes that use SDL types)
#include "SDL_stub.h"

// Basic xrCore types
#include "xrCore/xrCore.h"
#include "xrCore/_sphere.h"
#include "xrCore/_fbox.h"
#include "xrCore/_vector3d.h"
#include "xrCore/_matrix.h"
#include "xrCore/_color.h"
#include "xrCore/_flags.h"

// Global environment (GEnv) - must be before xrEngine/Render.h
#include "Include/xrAPI/xrAPI.h"

// ImGui types (ImTextureID)
#include "imgui/imgui.h"

// OpenGL ES 3.0+ types (GLuint, GLenum, etc.)
#include <GLES3/gl3.h>
// OpenGL ES 3.1+ for glProgramUniform* functions (separate shader objects)
#include <GLES3/gl31.h>

// GLAD compatibility layer for Android OpenGL ES
// Desktop OpenGL uses GLAD loader, but Android has native OpenGL ES
// Define GLAD feature flags to match desktop code
#define GLAD_GL_ARB_separate_shader_objects 1
// glProgramUniform* functions are available natively in OpenGL ES 3.1+

// DirectX-to-OpenGL type mappings for xrRenderGL compatibility
// These types are referenced in xrRender code but defined differently in OpenGL backend
using D3DCMPFUNC = GLenum;      // Comparison functions (GL_NEVER, GL_LESS, etc.)
using D3DSTENCILOP = GLenum;    // Stencil operations (GL_KEEP, GL_REPLACE, etc.)
using D3DBLEND = GLenum;        // Blend factors (GL_ZERO, GL_ONE, GL_SRC_ALPHA, etc.)
using D3DBLENDOP = GLenum;      // Blend operations (GL_FUNC_ADD, GL_FUNC_SUBTRACT, etc.)
using D3DCULL = GLenum;         // Culling modes (GL_FRONT, GL_BACK, GL_NONE)
using D3DFORMAT = GLenum;       // Texture formats (GL_RGBA8, GL_DEPTH_COMPONENT24, etc.)
using D3DPRIMITIVETYPE = GLenum; // Primitive types (GL_TRIANGLES, GL_LINES, etc.)
using D3DRENDERSTATETYPE = u32;  // Render state type (DirectX state IDs)
using D3DSAMPLERSTATETYPE = u32; // Sampler state type (DirectX sampler state IDs)

// DirectX primitive topology constants (map to OpenGL ES primitive types)
#define D3DPT_POINTLIST     GL_POINTS          // Point list
#define D3DPT_LINELIST      GL_LINES           // Line list
#define D3DPT_LINESTRIP     GL_LINE_STRIP      // Line strip
#define D3DPT_TRIANGLELIST  GL_TRIANGLES       // Triangle list (CRITICAL: used by FProgressive.cpp)
#define D3DPT_TRIANGLESTRIP GL_TRIANGLE_STRIP  // Triangle strip
#define D3DPT_TRIANGLEFAN   GL_TRIANGLE_FAN    // Triangle fan

// DirectX viewport structure
struct D3D_VIEWPORT {
    float TopLeftX;
    float TopLeftY;
    float Width;
    float Height;
    float MinDepth;
    float MaxDepth;
};

// Vertex element descriptor (DirectX D3D11_INPUT_ELEMENT_DESC equivalent)
// OpenGL ES 3.0+ uses vertex attribute pointers set via glVertexAttribPointer
struct VertexElement {
    u32 Stream;           // Vertex buffer binding index
    u32 Offset;           // Byte offset from start of vertex
    u32 Type;             // Data type (GL_FLOAT, GL_INT, etc.)
    u32 Method;           // Unused in OpenGL (DirectX vertex processing method)
    u32 Usage;            // Semantic usage (position, normal, texcoord, etc.)
    u32 UsageIndex;       // Semantic index for multiple attributes of same type
};

// Buffer handle types (OpenGL uses GLuint for all buffer objects)
using ConstantBufferHandle = GLuint;
using HostBufferHandle = GLuint;
using InputElementDesc = VertexElement;  // Alias for compatibility

// DirectX blend mode constants mapped to OpenGL equivalents
#define D3DBLEND_ZERO           GL_ZERO
#define D3DBLEND_ONE            GL_ONE
#define D3DBLEND_SRCCOLOR       GL_SRC_COLOR
#define D3DBLEND_INVSRCCOLOR    GL_ONE_MINUS_SRC_COLOR
#define D3DBLEND_SRCALPHA       GL_SRC_ALPHA
#define D3DBLEND_INVSRCALPHA    GL_ONE_MINUS_SRC_ALPHA
#define D3DBLEND_DESTALPHA      GL_DST_ALPHA
#define D3DBLEND_INVDESTALPHA   GL_ONE_MINUS_DST_ALPHA
#define D3DBLEND_DESTCOLOR      GL_DST_COLOR
#define D3DBLEND_INVDESTCOLOR   GL_ONE_MINUS_DST_COLOR
#define D3DBLEND_SRCALPHASAT    GL_SRC_ALPHA_SATURATE

// DirectX texture addressing modes mapped to OpenGL
#define D3DTADDRESS_WRAP        GL_REPEAT
#define D3DTADDRESS_MIRROR      GL_MIRRORED_REPEAT
#define D3DTADDRESS_CLAMP       GL_CLAMP_TO_EDGE
#define D3DTADDRESS_BORDER      GL_CLAMP_TO_BORDER

// DirectX texture filter modes mapped to OpenGL
#define D3DTEXF_NONE            GL_NEAREST
#define D3DTEXF_POINT           GL_NEAREST
#define D3DTEXF_LINEAR          GL_LINEAR
#define D3DTEXF_ANISOTROPIC     GL_LINEAR  // Anisotropic filtering set separately

// DirectX comparison functions mapped to OpenGL
#define D3DCMP_NEVER            GL_NEVER
#define D3DCMP_LESS             GL_LESS
#define D3DCMP_EQUAL            GL_EQUAL
#define D3DCMP_LESSEQUAL        GL_LEQUAL
#define D3DCMP_GREATER          GL_GREATER
#define D3DCMP_NOTEQUAL         GL_NOTEQUAL
#define D3DCMP_GREATEREQUAL     GL_GEQUAL
#define D3DCMP_ALWAYS           GL_ALWAYS

// DirectX stencil operations mapped to OpenGL
#define D3DSTENCILOP_KEEP       GL_KEEP
#define D3DSTENCILOP_ZERO       GL_ZERO
#define D3DSTENCILOP_REPLACE    GL_REPLACE
#define D3DSTENCILOP_INCRSAT    GL_INCR
#define D3DSTENCILOP_DECRSAT    GL_DECR
#define D3DSTENCILOP_INVERT     GL_INVERT
#define D3DSTENCILOP_INCR       GL_INCR_WRAP
#define D3DSTENCILOP_DECR       GL_DECR_WRAP

// DirectX culling modes mapped to OpenGL
#define D3DCULL_NONE            GL_NONE
#define D3DCULL_CW              0x0900  // GL_CW (not standard in ES, placeholder)
#define D3DCULL_CCW             0x0901  // GL_CCW (not standard in ES, placeholder)

// DirectX color write enable flags (mapped to OpenGL color mask)
#define D3DCOLORWRITEENABLE_RED     0x00000001  // GL_TRUE for red channel
#define D3DCOLORWRITEENABLE_GREEN   0x00000002  // GL_TRUE for green channel
#define D3DCOLORWRITEENABLE_BLUE    0x00000004  // GL_TRUE for blue channel
#define D3DCOLORWRITEENABLE_ALPHA   0x00000008  // GL_TRUE for alpha channel

// DirectX Flexible Vertex Format (FVF) flags (legacy, unused in modern OpenGL ES)
#define D3DFVF_XYZ              0x002   // Position (x, y, z)
#define D3DFVF_XYZRHW           0x004   // Transformed position (x, y, z, rhw)
#define D3DFVF_NORMAL           0x010   // Normal vector
#define D3DFVF_DIFFUSE          0x040   // Diffuse color
#define D3DFVF_SPECULAR         0x080   // Specular color
#define D3DFVF_TEX1             0x100   // 1 texture coordinate set
#define D3DFVF_TEX2             0x200   // 2 texture coordinate sets
#define D3DFVF_TEX3             0x300   // 3 texture coordinate sets
#define D3DFVF_TEX4             0x400   // 4 texture coordinate sets

// DirectX texture stage state (legacy fixed-function pipeline)
// OpenGL ES 3.0+ uses shaders, these are for compatibility only
#define D3DTSS_COLOROP          0
#define D3DTSS_COLORARG1        1
#define D3DTSS_COLORARG2        2
#define D3DTSS_ALPHAOP          3
#define D3DTSS_ALPHAARG1        4
#define D3DTSS_ALPHAARG2        5
#define D3DTSS_COLORARG0        6
#define D3DTSS_ALPHAARG0        7

// DirectX texture operation (legacy fixed-function)
#define D3DTOP_DISABLE          0
#define D3DTOP_SELECTARG1       1
#define D3DTOP_SELECTARG2       2
#define D3DTOP_MODULATE         3
#define D3DTOP_ADD              4

// DirectX texture arguments (legacy fixed-function)
#define D3DTA_DIFFUSE           0x00000000  // Select diffuse color
#define D3DTA_CURRENT           0x00000001  // Select result of previous stage
#define D3DTA_TEXTURE           0x00000002  // Select texture color
#define D3DTA_TFACTOR           0x00000003  // Select TFACTOR (texture factor constant)
#define D3DTA_SPECULAR          0x00000004  // Select specular color
#define D3DTA_TEMP              0x00000005  // Select temporary register
#define D3DTA_CONSTANT          0x00000006  // Select per-stage constant
#define D3DTA_COMPLEMENT        0x00000010  // Take 1.0 - x
#define D3DTA_ALPHAREPLICATE    0x00000020  // Replicate alpha to color components

// DirectX vertex declaration types (D3DDECLTYPE_*)
#define D3DDECLTYPE_FLOAT1      0   // 1D float
#define D3DDECLTYPE_FLOAT2      1   // 2D float vector
#define D3DDECLTYPE_FLOAT3      2   // 3D float vector
#define D3DDECLTYPE_FLOAT4      3   // 4D float vector
#define D3DDECLTYPE_D3DCOLOR    4   // 4D packed ARGB color
#define D3DDECLTYPE_UBYTE4      5   // 4D unsigned byte
#define D3DDECLTYPE_SHORT2      6   // 2D signed short
#define D3DDECLTYPE_SHORT4      7   // 4D signed short
#define D3DDECLTYPE_UBYTE4N     8   // Normalized 4D unsigned byte
#define D3DDECLTYPE_SHORT2N     9   // Normalized 2D signed short
#define D3DDECLTYPE_SHORT4N     10  // Normalized 4D signed short
#define D3DDECLTYPE_USHORT2N    11  // Normalized 2D unsigned short
#define D3DDECLTYPE_USHORT4N    12  // Normalized 4D unsigned short
#define D3DDECLTYPE_UDEC3       13  // 3D unsigned 10-10-10 format
#define D3DDECLTYPE_DEC3N       14  // Normalized 3D signed 10-10-10 format
#define D3DDECLTYPE_FLOAT16_2   15  // 2D 16-bit float vector
#define D3DDECLTYPE_FLOAT16_4   16  // 4D 16-bit float vector

// DirectX vertex declaration method (D3DDECLMETHOD_*)
#define D3DDECLMETHOD_DEFAULT   0   // Default method
#define D3DDECLMETHOD_PARTIALU  1   // Partial U
#define D3DDECLMETHOD_PARTIALV  2   // Partial V
#define D3DDECLMETHOD_CROSSUV   3   // Cross UV
#define D3DDECLMETHOD_UV        4   // UV
#define D3DDECLMETHOD_LOOKUP    5   // Lookup
#define D3DDECLMETHOD_LOOKUPPRESAMPLED 6 // Lookup presampled

// DirectX vertex declaration usage (D3DDECLUSAGE_*)
#define D3DDECLUSAGE_POSITION   0   // Position
#define D3DDECLUSAGE_BLENDWEIGHT 1  // Blend weight
#define D3DDECLUSAGE_BLENDINDICES 2 // Blend indices
#define D3DDECLUSAGE_NORMAL     3   // Normal
#define D3DDECLUSAGE_PSIZE      4   // Point size
#define D3DDECLUSAGE_TEXCOORD   5   // Texture coordinate
#define D3DDECLUSAGE_TANGENT    6   // Tangent
#define D3DDECLUSAGE_BINORMAL   7   // Binormal
#define D3DDECLUSAGE_TESSFACTOR 8   // Tessellation factor
#define D3DDECLUSAGE_POSITIONT  9   // Transformed position
#define D3DDECLUSAGE_COLOR      10  // Color
#define D3DDECLUSAGE_FOG        11  // Fog
#define D3DDECLUSAGE_DEPTH      12  // Depth
#define D3DDECLUSAGE_SAMPLE     13  // Sample

// DirectX vertex declaration end marker
#define D3DDECL_END() {0xFF, 0, 0xFF, 0, 0, 0}

// DirectX render states (legacy fixed-function pipeline state)
#define D3DRS_TEXTUREFACTOR         0   // Texture factor constant (RGBA)
#define D3DRS_COLORWRITEENABLE      1   // Color write enable mask
#define D3DRS_COLORWRITEENABLE1     2   // Color write enable mask for RT1
#define D3DRS_COLORWRITEENABLE2     3   // Color write enable mask for RT2
#define D3DRS_COLORWRITEENABLE3     4   // Color write enable mask for RT3
#define D3DRS_ZFUNC                 5   // Depth comparison function
#define D3DRS_ZWRITEENABLE          6   // Depth write enable
#define D3DRS_ALPHABLENDENABLE      7   // Alpha blending enable
#define D3DRS_SRCBLEND              8   // Source blend factor
#define D3DRS_DESTBLEND             9   // Destination blend factor
#define D3DRS_SRCBLENDALPHA         10  // Source alpha blend factor
#define D3DRS_DESTBLENDALPHA        11  // Destination alpha blend factor
#define D3DRS_ALPHATESTENABLE       12  // Alpha test enable
#define D3DRS_ALPHAREF              13  // Alpha reference value
#define D3DRS_LIGHTING              14  // Lighting enable (fixed-function)
#define D3DRS_FOGENABLE             15  // Fog enable

// DirectX sampler states (texture sampling parameters)
#define D3DSAMP_ADDRESSU            0   // U coordinate addressing mode
#define D3DSAMP_ADDRESSV            1   // V coordinate addressing mode
#define D3DSAMP_ADDRESSW            2   // W coordinate addressing mode
#define D3DSAMP_BORDERCOLOR         3   // Border color for D3DTADDRESS_BORDER
#define D3DSAMP_MAGFILTER           4   // Magnification filter
#define D3DSAMP_MINFILTER           5   // Minification filter
#define D3DSAMP_MIPFILTER           6   // Mipmap filter
#define D3DSAMP_MIPMAPLODBIAS       7   // Mipmap LOD bias
#define D3DSAMP_MAXMIPLEVEL         8   // Maximum mipmap level
#define D3DSAMP_MAXANISOTROPY       9   // Maximum anisotropy level

// DirectX texture stage states (texture coordinate transformation)
#define D3DTSS_TEXTURETRANSFORMFLAGS 10 // Texture coordinate transform flags
#define D3DTSS_TEXCOORDINDEX        11  // Texture coordinate index

// DirectX texture coordinate index generation (D3DTSS_TEXCOORDINDEX values)
#define D3DTSS_TCI_PASSTHRU                     0x00000000  // Use vertex texture coordinates
#define D3DTSS_TCI_CAMERASPACEPOSITION          0x00010000  // Generate from camera space position
#define D3DTSS_TCI_CAMERASPACENORMAL            0x00020000  // Generate from camera space normal
#define D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR  0x00030000  // Generate from camera space reflection

// DirectX texture transform flags
#define D3DTTFF_DISABLE             0   // Disable texture coordinate transformation
#define D3DTTFF_COUNT1              1   // 1D texture coordinates
#define D3DTTFF_COUNT2              2   // 2D texture coordinates
#define D3DTTFF_COUNT3              3   // 3D texture coordinates
#define D3DTTFF_COUNT4              4   // 4D texture coordinates
#define D3DTTFF_PROJECTED           256 // Projected texture coordinates (divide by w)

// DirectX texture filter modes (additional)
#define D3DTEXF_GAUSSIANQUAD        GL_LINEAR  // Gaussian quad filter (fallback to linear)

// Render configuration variables (anisotropic filtering level)
extern int ps_r__tf_Anisotropic;

// Render state object (DirectX ID3D11SamplerState equivalent)
// In OpenGL we don't have state objects, just direct state setting
using ID3DState = void*;        // Placeholder - OpenGL doesn't use state objects

// Vertex buffer/index buffer handles (OpenGL uses GLuint for buffer objects)
using VertexBufferHandle = GLuint;
using IndexBufferHandle = GLuint;

// Forward declarations for buffer staging classes (defined in BufferUtils.h)
namespace xray::render::RENDER_NAMESPACE {
    class VertexStagingBuffer;
    class IndexStagingBuffer;
}

// DirectX vertex element descriptor (D3DVERTEXELEMENT9 equivalent for OGF_GContainer_Vertices.hpp)
using D3DVERTEXELEMENT9 = VertexElement;

// IRenderVisual and IKinematics interfaces (needed for covariance in SkeletonCustom.h)
#include "Include/xrRender/RenderVisual.h"
#include "Include/xrRender/Kinematics.h"

// Rendering types from xrEngine
#include "xrEngine/Render.h"
#include "xrRender_R2/r2_types.h"  // Shared GL/R2 render-target names used by OpenXRay blenders
#include "xrEngine/device.h"        // CRenderDevice (Device object)

// Property system for shader blenders (xrP_TOKEN, xrP_Integer, xrP_BOOL)
#include "xrEngine/Properties.h"

// Collision detection (xrXRC - ray casting)
#include "xrCDB/xrXRC.h"

// Task system (parallel task execution)
#include "xrCore/Threading/Task.hpp"

// Visibility data structure (vis_data with sphere, box, clear() method)
#include "xrEngine/vis_common.h"

// Collision detection definitions (collide::ray_cache needed by LightTrack.h)
#include "xrCDB/xr_collide_defs.h"

// Hardware capabilities class (needed by CHW)
#include "Layers/xrRender/HWCaps.h"

// Blender CLASS_ID constants (B_DEFAULT, B_SHADOW_WORLD, etc.)
// Blender_CLSID.h uses make_clsid() from namespace xray inside namespace xray::render::RENDER_NAMESPACE
// Import make_clsid into render namespace BEFORE including the header
namespace xray::render {
    using xray::make_clsid;
}
#include "Layers/xrRender/Blender_CLSID.h"

// Global hardware object (CHW - hardware abstraction, defined in xrRenderGL)
// Forward declare CHW with IMM_CTX_ID - MUST be defined BEFORE R_Backend.h
// Full CHW definition is in glHW.h (included after R_Backend.h to avoid circular dependency)
namespace xray::render::RENDER_NAMESPACE {
    class CHW {
    public:
        static constexpr u32 IMM_CTX_ID = 0;  // Immediate context ID constant
        CHWCaps Caps;  // Hardware capabilities (needed by Blender_Recorder.cpp)
    };
}

// Shader resource pointers (ref_shader, ref_geom, ref_constant)
// Include actual definitions from xrRender, not void* placeholders
#include "Layers/xrRender/Shader.h"
#include "Layers/xrRender/Blender.h"           // IBlender base class
#include "Layers/xrRender/Blender_Recorder.h"  // CBlender_Compile definition

// GL renderer helper from the upstream xrRenderPC_GL precompiled header.
namespace xray::render::RENDER_NAMESPACE {
    inline void jitter(CBlender_Compile& C)
    {
        C.r_Sampler("jitter0", JITTER(0), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
        C.r_Sampler("jitter1", JITTER(1), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
        C.r_Sampler("jitter2", JITTER(2), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
        C.r_Sampler("jitter3", JITTER(3), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    }
}

// CResourceManager - shader/texture resource manager (real implementation, not stub!)
#include "Layers/xrRender/xrRender_console.h"
#include "Layers/xrRender/ResourceManager.h"

#include "Layers/xrRender/R_Backend.h"         // CBackend definition (uses CHW::IMM_CTX_ID)

// Visual classes - dxRender_Visual base class (needed by light_smapvis.h - fixes 12 errors)
#include "Layers/xrRender/FBasicVisual.h"

// R_feedback base class (needed by light_smapvis.h line 5 - fixes 8 errors in Build #224)
// Declare only the feedback interface here. Including the upstream scene-graph
// header also defines R_dsgraph_structure, which conflicts with the Android
// compatibility structure declared below.
namespace xray::render::RENDER_NAMESPACE {
    class R_feedback {
    public:
        virtual void rfeedback_static(dxRender_Visual* V) = 0;
    };
}

// DirectX extensions - Flight structure (needed by light.h line 42 - fixes 20 errors in Build #223)
#include "Common/_d3d_extensions.h"

// Light system - full light class definition (needed by LightTrack.cpp, Light_Package.cpp - fixes 76 errors)
#include "Layers/xrRender/light.h"
#include "Layers/xrRender/Light_DB.h"

// FSlideWindowItem forward declaration (needed by FTreeVisual.cpp - fixes 4 errors)
struct FSlideWindowItem;

// Forward declarations for RImplementation subsystems
namespace xray::render::RENDER_NAMESPACE {
    class CDetailManager;  // Detail geometry (grass, small objects)
    
    // CHOM stub - minimal implementation for compilation (real HOM.cpp will be excluded)
    class CHOM {
    public:
        // Visibility check method (used by DetailManager.cpp:321)
        bool visible(const vis_data&) const { return true; }  // Always visible stub
        bool visible(const Fbox3&) const { return true; }
        bool IsSOccluded(void*, float) const { return false; }
    };

    // Simple profiler timer stub for BasicStats (Begin/End methods)
    struct CStatTimer {
        void Begin() {}
        void End() {}
    };
}

// Global game level pointer (full definition needed for ObjectSpace access in DetailManager_Decompress.cpp)
#include "xrEngine/IGame_Level.h"
extern IGame_Level* g_pGameLevel;

// R_dsgraph_structure - scene graph context with command list (RCache backend)
// RCache macro expands to: RImplementation.get_imm_context().cmd_list
namespace xray::render::RENDER_NAMESPACE {
    struct R_dsgraph_structure {
        CBackend cmd_list;  // Command list for RCache.set_Geometry(), RCache.Render(), etc.
    };

    // RImplementation wrapper class with Resources and subsystems
    // xrRender code expects RImplementation to have Resources field and blender methods
    class CRenderImplementation {
    public:
        CResourceManager* Resources = nullptr;  // Shader/texture resource manager

        // Rendering subsystems (used by various xrRender modules)
        CDetailManager* Details = nullptr;      // Detail geometry manager
        CHOM HOM;                                // Hardware occlusion manager (object, not pointer!)

        // Dynamic lighting system (used by Light_DB.cpp, LightTrack.cpp)
        void* L_Dynamic = nullptr;              // Dynamic lights container
        bool is_sun_static() const { return false; }  // Sun is dynamic in this Android renderer shim
        int m_MSAASample = 0;                 // Current OpenGL multisample mode
        CLight_DB Lights;                        // Light database used by sun/light queries

        // Render state flags (legacy DirectX 9 renderer)
        struct {
            u32 ffp = 0;                    // Fixed-function pipeline flags
            u32 no_detail_textures = 0;     // Disable detail textures flag
            bool advancedpp = false;         // Advanced post-processing feature flag
        } o;

        // Vertex buffers for common geometry (used by FLOD.cpp, FSkinned.cpp)
        struct {
            VertexBufferHandle Buffer() { return 0; }  // Return VB handle (unsigned int), not void*
        } Vertex;
        IndexBufferHandle QuadIB = 0;  // Quad index buffer (2 triangles = 6 indices)

        // Basic rendering statistics (profiler timers)
        struct {
            u32 Primitives = 0;             // Number of primitives rendered
            u32 Vertices = 0;               // Number of vertices rendered
            CStatTimer DetailCache;         // Detail cache profiler
            CStatTimer DetailVisibility;    // Detail visibility profiler
            CStatTimer DetailRender;        // Detail render profiler
        } BasicStats;

        // Blender management methods (shader material compilation)
        IBlender* blender_create(CLASS_ID cls);
        void blender_destroy(IBlender*& B);

        // Shader management method (used by FBasicVisual.cpp)
        ref_shader getShader(int) { return ref_shader(); }

        // Visual model management methods (used by FHierrarhyVisual.cpp)
        void* getVisual(int) { return nullptr; }
        void model_Delete(void*, bool = false) {}  // 2nd param: bDiscard (default false)
        void* model_CreateChild(const char*, IReader*) { return nullptr; }
        void* model_Duplicate(void*) { return nullptr; }

        // Vertex/Index buffer accessors (used by FVisual.cpp, FTreeVisual.cpp)
        // Return VertexStagingBuffer/IndexStagingBuffer pointers, not raw handles
        VertexStagingBuffer* getVB(int, bool = false) { return nullptr; }
        IndexStagingBuffer* getIB(int, bool = false) { return nullptr; }
        VertexElement* getVB_Format(int, bool = false) { return nullptr; }
        bool IsFastGeomSupported() { return false; }

        // Shader skinning option (used by FSkinned.cpp)
        void shader_option_skinning(int) {}

        // SWI (slide window item) accessor (used by FTreeVisual.cpp)
        // Returns FSlideWindowItem*, not void*
        FSlideWindowItem* getSWI(int) { return nullptr; }

        // Immediate context accessor (for RCache macro)
        // RCache macro expands to: RImplementation.get_imm_context().cmd_list
        R_dsgraph_structure imm_context;
        R_dsgraph_structure& get_imm_context() { return imm_context; }
    };
}

// Global renderer objects (forward declarations for OpenGL ES)
// These are defined in xrRenderGL backend but referenced in shared xrRender code
namespace xray::render::gl {
    class CRenderTarget;
}

// R1 renderer flags (legacy DirectX 9 renderer settings, unused in OpenGL ES)
extern Flags32 ps_r1_flags;
#define R1FLAG_FFP_LIGHTMAPS    (1 << 0)
#define R1FLAG_DLIGHTS          (1 << 1)

// Global renderer implementation object (defined in xrRenderGL, stubbed here)
// Use macro to dereference pointer so code can use . operator instead of ->
extern xray::render::RENDER_NAMESPACE::CRenderImplementation* _RImplementation_ptr;
#define RImplementation (*_RImplementation_ptr)

// Global hardware object (defined in xrRenderGL glHW.cpp)
extern xray::render::RENDER_NAMESPACE::CHW HW;

// Device object - CRenderDevice (main engine device)
// Device is used extensively in xrRender for timing, camera, viewport access
extern CRenderDevice* _Device_ptr;
#define Device (*_Device_ptr)

// Global render configuration variables (detail rendering, render states)
extern float ps_r__Detail_density;      // Detail density multiplier (grass, small objects)
extern float ps_r__Detail_height;       // Detail height threshold
extern int ps_current_detail_height;    // Current detail LOD height
extern u32 rsDrawDetails;               // Render state: draw detail geometry flag
extern Flags32 psDeviceFlags;           // Device configuration flags
extern u32 ps_r1_force_geomx;           // R1 force fast geometry flag (used by FVisual.cpp) - u32 not Flags32!

// Tree rendering configuration (used by FTreeVisual.cpp)
extern float ps_r__Tree_SBC;            // Tree speed-tree billboard clouds - float not Flags32!

// R2/R3/R4 renderer configuration (dynamic lighting, hemispheric lighting, sun)
extern int ps_r2_dhemi_count;           // Dynamic hemispheric light sample count
extern float ps_r2_dhemi_sky_scale;     // Dynamic hemispheric sky light scale
extern float ps_r2_sun_lumscale;        // Sun luminance scale (used by Light_DB.cpp)

// Game mode configuration (used by Light_DB.cpp)
extern bool ShadowOfChernobylMode;      // Shadow of Chernobyl compatibility mode

// Smart pointers for render objects (intrusive reference counting)
// Used by SkeletonCustom.h for CSkeletonWallmark management
template<typename T>
class intrusive_ptr {
    T* ptr = nullptr;
public:
    intrusive_ptr() = default;
    intrusive_ptr(T* p) : ptr(p) {}
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
};

// Intrusive base class for reference counting (used by CSkeletonWallmark)
class intrusive_base {
public:
    virtual ~intrusive_base() = default;
};

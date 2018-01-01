#ifndef GLENUMS_H
#define GLENUMS_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

using GLId = GLuint;

enum GL_Enum
{
    ActiveUniforms       = GL_ACTIVE_UNIFORMS,
    CompileStatus        = GL_COMPILE_STATUS,
    Fill                 = GL_FILL,
    InfoLogLength        = GL_INFO_LOG_LENGTH,
    Line                 = GL_LINE,
    LinkStatus           = GL_LINK_STATUS,
    MaxTextureImageUnits = GL_MAX_TEXTURE_IMAGE_UNITS,
    ReadOnly             = GL_READ_ONLY,
    ReadWrite            = GL_READ_WRITE,
    UnPackAlignment      = GL_UNPACK_ALIGNMENT,
    Viewport             = GL_VIEWPORT,
    WriteOnly            = GL_WRITE_ONLY
};

enum class GL_Primitive
{
    Points        = GL_POINTS,
    Lines         = GL_LINES,
    LineStrip     = GL_LINE_STRIP,
    LineLoop      = GL_LINE_LOOP,
    Triangles     = GL_TRIANGLES,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan   = GL_TRIANGLE_FAN,
    Quads         = GL_QUADS,
    QuadStrip     = GL_QUAD_STRIP,
    Polygon       = GL_POLYGON
};

enum class GL_Test
{
    Blend    = GL_BLEND,
    Depth    = GL_DEPTH_TEST,
    Stencil  = GL_STENCIL_TEST,
    Scissor  = GL_SCISSOR_TEST,
    Alpha    = GL_ALPHA_TEST,
    CullFace = GL_CULL_FACE
};

enum class GL_UsageHint
{
    StreamDraw  = GL_STREAM_DRAW,
    StreamRead  = GL_STREAM_READ,
    StreamCopy  = GL_STREAM_COPY,
    StaticDraw  = GL_STATIC_DRAW,
    StaticRead  = GL_STATIC_READ,
    StaticCopy  = GL_STATIC_COPY,
    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY
};

enum class GL_Face
{
    Front        = GL_FRONT,
    Back         = GL_BACK,
    FrontAndBack = GL_FRONT_AND_BACK
};

enum class GL_BindTarget
{
    None           = 0,
    Texture2D      = GL_TEXTURE_2D,
    ShaderProgram  = GL_SHADER,
    Framebuffer    = GL_FRAMEBUFFER,
    VAO            = GL_VERTEX_ARRAY,
    VBO            = GL_ARRAY_BUFFER,
    UniformBuffer  = GL_UNIFORM_BUFFER
};

enum class GL_DataType
{
    Byte          = GL_BYTE,
    UnsignedByte  = GL_UNSIGNED_BYTE,
    Short         = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int           = GL_INT,
    UnsignedInt   = GL_UNSIGNED_INT,
    Float         = GL_FLOAT,
    Double        = GL_DOUBLE,
    UnsignedInt_24_8 = GL_UNSIGNED_INT_24_8,
    Vector2       = GL_FLOAT_VEC2,
    Vector3       = GL_FLOAT_VEC3,
    Vector4       = GL_FLOAT_VEC4,
    Matrix3       = GL_FLOAT_MAT3,
    Matrix4       = GL_FLOAT_MAT4
};

enum class GL_ShaderType
{
    Vertex   = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

enum class GL_ColorComp
{
    RGB  = GL_RGB,
    RGBA = GL_RGBA,
    Depth = GL_DEPTH_COMPONENT,
    DepthStencil = GL_DEPTH_STENCIL,
    StencilIndex = GL_STENCIL_INDEX
};

enum class GL_ColorFormat
{
    RGBA_UByte8          = GL_RGBA8,
    RGBA_Float16         = GL_RGBA16F,
    RGB10_A2_UByte       = GL_RGB10_A2,
    RGBA_Float32         = GL_RGBA32F,
    Depth24_Stencil8     = GL_DEPTH24_STENCIL8
};

enum class GL_ViewProjMode
{
    UseBoth            = 0,
    OnlyFixAspectRatio = 1,
    IgnoreBoth         = 2,
    IgnoreBothAndModel = 3
};

enum class GL_TextureTarget
{
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    Texture3D = GL_TEXTURE_3D,
    TextureCubeMap = GL_TEXTURE_CUBE_MAP
};

enum class GL_FramebufferTarget
{
    Draw = GL_DRAW_FRAMEBUFFER,
    Read = GL_READ_FRAMEBUFFER,
    ReadDraw = GL_FRAMEBUFFER
};
enum class GL_RenderbufferTarget
{
    Renderbuffer = GL_RENDERBUFFER
};

enum class GL_RenderbufferFormat
{
    Depth = GL_DEPTH,
    Stencil = GL_STENCIL,
    Depth24_Stencil8 = GL_DEPTH24_STENCIL8
};

enum class GL_BufferBit
{
    Color    = GL_COLOR_BUFFER_BIT,
    Depth    = GL_DEPTH_BUFFER_BIT,
    Stencil  = GL_STENCIL_BUFFER_BIT,
    Scissor  = GL_SCISSOR_BIT
};

enum class GL_WrapMode
{
    Repeat = GL_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER
};
enum class GL_WrapCoord
{
    WrapS = GL_TEXTURE_WRAP_S,
    WrapT = GL_TEXTURE_WRAP_T,
    WrapR = GL_TEXTURE_WRAP_R
};

enum class GL_FilterMode
{
    Nearest = GL_NEAREST,
    Bilinear = GL_LINEAR,
    Trilinear_NN = GL_NEAREST_MIPMAP_NEAREST,
    Trilinear_NL = GL_NEAREST_MIPMAP_LINEAR,
    Trilinear_LN = GL_LINEAR_MIPMAP_NEAREST,
    Trilinear_LL = GL_LINEAR_MIPMAP_LINEAR
};
enum class GL_FilterMagMin
{
    Mag = GL_TEXTURE_MAG_FILTER,
    Min = GL_TEXTURE_MIN_FILTER
};

enum class GL_StencilOperation
{
    Zero     = GL_ZERO,
    Keep     = GL_KEEP,
    Replace  = GL_REPLACE,
    Incr     = GL_INCR,
    IncrWrap = GL_INCR_WRAP,
    Decr     = GL_DECR,
    DecrWrap = GL_DECR_WRAP,
    Invert   = GL_INVERT
};

enum class GL_BlendFactor
{
    None             = GL_NONE,
    Zero             = GL_ZERO,
    One              = GL_ONE,
    SrcColor         = GL_SRC_COLOR,
    OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
    DstColor         = GL_DST_COLOR,
    OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
    SrcAlpha         = GL_SRC_ALPHA,
    OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
    DstAlpha         = GL_DST_ALPHA,
    OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA
};

enum class GL_Function
{
    Never    = GL_NEVER,
    Less     = GL_LESS,
    LEqual   = GL_LEQUAL,
    Greater  = GL_GREATER,
    GEqual   = GL_GEQUAL,
    Equal    = GL_EQUAL,
    NotEqual = GL_NOTEQUAL,
    Always   = GL_ALWAYS
};

enum class GL_Attachment
{
    Color0       = GL_COLOR_ATTACHMENT0,
    Color1       = GL_COLOR_ATTACHMENT1,
    Color2       = GL_COLOR_ATTACHMENT2,
    Color3       = GL_COLOR_ATTACHMENT3,
    Color4       = GL_COLOR_ATTACHMENT4,
    Color5       = GL_COLOR_ATTACHMENT5,
    Color6       = GL_COLOR_ATTACHMENT6,
    Color7       = GL_COLOR_ATTACHMENT7,
    DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
    Stencil      = GL_STENCIL_ATTACHMENT,
    Depth        = GL_DEPTH_ATTACHMENT
};

NAMESPACE_BANG_END

#endif // GLENUMS_H

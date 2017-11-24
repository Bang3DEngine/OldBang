#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Bang/Map.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"

NAMESPACE_BANG_BEGIN

#ifdef DEBUG
#define GL_ClearError() GL::ClearError()
#define GL_CheckError() ASSERT(GL::CheckError(__LINE__, __FUNCTION__, __FILE__) )
#else
#define GL_ClearError() // Empty
#define GL_CheckError() // Empty
#endif

using GLId = GLuint;

FORWARD class VAO;
FORWARD class Texture;
FORWARD class GLObject;
FORWARD class GLUniforms;
FORWARD class ShaderProgram;
FORWARD class IUniformBuffer;

class GL
{
public:
    using Enum = GLenum;

    enum
    {
        Viewport  = GL_VIEWPORT,
        Blend     = GL_BLEND,
        WriteOnly = GL_WRITE_ONLY,
        ReadOnly  = GL_READ_ONLY,
        ReadWrite = GL_READ_WRITE
    };

    enum class Primitives
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

    enum Test
    {
        Depth    = GL_DEPTH_TEST,
        Stencil  = GL_STENCIL_TEST,
        Scissor  = GL_SCISSOR_TEST,
        Alpha    = GL_ALPHA_TEST,
        CullFace = GL_CULL_FACE
    };

    enum class UsageHint
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

    enum class Face
    {
        Front        = GL_FRONT,
        Back         = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK,
        None         = GL_NONE
    };

    enum class BindTarget
    {
        None           = 0,
        Texture2D      = GL_TEXTURE_2D,
        ShaderProgram  = GL_SHADER,
        Framebuffer    = GL_FRAMEBUFFER,
        VAO            = GL_VERTEX_ARRAY,
        VBO            = GL_ARRAY_BUFFER,
        UniformBuffer  = GL_UNIFORM_BUFFER
    };

    enum class DataType
    {
        Byte          = GL_BYTE,
        UnsignedByte  = GL_UNSIGNED_BYTE,
        Short         = GL_SHORT,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Int           = GL_INT,
        UnsignedInt   = GL_UNSIGNED_INT,
        Float         = GL_FLOAT,
        Double        = GL_DOUBLE,
        Vector2       = GL_FLOAT_VEC2,
        Vector3       = GL_FLOAT_VEC3,
        Vector4       = GL_FLOAT_VEC4,
        Matrix3       = GL_FLOAT_MAT3,
        Matrix4       = GL_FLOAT_MAT4
    };

    enum class ShaderType
    {
        Vertex   = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    enum class ColorComp
    {
        RGB  = GL_RGB,
        RGBA = GL_RGBA,
        Depth = GL_DEPTH_COMPONENT,
        DepthStencil = GL_DEPTH_STENCIL,
        StencilIndex = GL_STENCIL_INDEX
    };

    enum class ColorFormat
    {
        RGBA_UByte8          = GL_RGBA8,
        RGBA_Float16         = GL_RGBA16F,
        RGB10_A2_UByte       = GL_RGB10_A2,
        RGBA_Float32         = GL_RGBA32F,
        Depth24_Stencil8     = GL_DEPTH24_STENCIL8
    };

    enum class ViewProjMode
    {
        UseBoth            = 0,
        OnlyFixAspectRatio = 1,
        IgnoreBoth         = 2,
        IgnoreBothAndModel = 3
    };

    enum class TextureTarget
    {
        Texture1D = GL_TEXTURE_1D,
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP
    };

    enum class FramebufferTarget
    {
        Draw = GL_DRAW_FRAMEBUFFER,
        Read = GL_READ_FRAMEBUFFER,
        ReadDraw = GL_FRAMEBUFFER
    };
    enum class RenderbufferTarget
    {
        Renderbuffer = GL_RENDERBUFFER
    };

    enum class RenderbufferFormat
    {
        Depth = GL_DEPTH,
        Stencil = GL_STENCIL,
        Depth24_Stencil8 = GL_DEPTH24_STENCIL8
    };

    enum class BufferBit
    {
        Color    = GL_COLOR_BUFFER_BIT,
        Depth    = GL_DEPTH_BUFFER_BIT,
        Stencil  = GL_STENCIL_BUFFER_BIT,
        Scissor  = GL_SCISSOR_BIT
    };

    enum class WrapMode
    {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER
    };
    enum class WrapCoord
    {
        WrapS = GL_TEXTURE_WRAP_S,
        WrapT = GL_TEXTURE_WRAP_T,
        WrapR = GL_TEXTURE_WRAP_R
    };

    enum class FilterMode
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        Trilinear_NN = GL_NEAREST_MIPMAP_NEAREST,
        Trilinear_NL = GL_NEAREST_MIPMAP_LINEAR,
        Trilinear_LN = GL_LINEAR_MIPMAP_NEAREST,
        Trilinear_LL = GL_LINEAR_MIPMAP_LINEAR
    };
    enum class FilterMagMin
    {
        Mag = GL_TEXTURE_MAG_FILTER,
        Min = GL_TEXTURE_MIN_FILTER
    };

    enum class StencilOperation
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

    enum class BlendFactor
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

    enum class Function
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

    enum class Attachment
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

    template <class T>
    struct GLSLVar
    {
        String name = ""; T value;
        GLSLVar(const String &_name, const T &_value)
            : name(_name), value(_value) {}
        GLSLVar() {}
    };

    static void ClearError();
    static bool CheckError(int line = 0, const String &func = "",
                           const String &file = "");
    static bool CheckFramebufferError();

    static void Clear(GL::BufferBit bufferBit);
    static void ClearColorBuffer(const Color& clearColor = Color::Zero,
                                 bool clearR = true, bool clearG = true,
                                 bool clearB = true, bool clearA = true);
    static void ClearDepthBuffer(float clearDepth = 1.0f);
    static void ClearStencilBuffer(int stencilValue = 0);

    static void Enable (GL::Enum glEnum);
    static void Disable(GL::Enum glEnum);

    static void EnableVertexAttribArray(int location);
    static void DisableVertexAttribArray(int location);
    static void VertexAttribPointer(int location,
                                    int dataComponentsCount,
                                    GL::DataType dataType,
                                    bool dataNormalized,
                                    int dataStride,
                                    int dataOffset);

    static void PolygonMode(GL::Face face, GL::Enum mode);

    static GLvoid* MapBuffer(GL::BindTarget target, Enum access);
    static void UnMapBuffer(GL::BindTarget target);

    static void BlendFunc(BlendFactor srcFactor, BlendFactor dstFactor);

    static void BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1,
                                int dstX0, int dstY0, int dstX1, int dstY1,
                                GL::FilterMode filterMode,
                                GL::BufferBit bufferBitMask);
    static void BlitFramebuffer(const Recti &srcRect, const Recti &dstRect,
                                GL::FilterMode filterMode,
                                GL::BufferBit bufferBitMask);

    static GLId CreateShader(GL::ShaderType shaderType);
    static void ShaderSource(GLId shaderId, const String &sourceCode);
    static bool CompileShader(GLId shaderId);
    static void DeleteShader(GLId shaderId);


    static GLId   CreateProgram();
    static void   AttachShader(GLId programId, GLId shaderId);
    static bool   LinkProgram(GLId programId);
    static void   BindAttribLocation(GLId programId, int location,
                                     const String &attribName);
    static void   BindFragDataLocation(GLId programId, int location,
                                       const String &fragDataName);
    static void   DeleteProgram(GLId programId);

    static void FramebufferTexture2D(GL::FramebufferTarget target,
                                     GL::Attachment attachment,
                                     GL::TextureTarget texTarget,
                                     GLId textureId);
    static void BindRenderbuffer(GL::RenderbufferTarget target,
                                 GLId renderbufferId);
    static void RenderbufferStorage(GL::RenderbufferTarget target,
                                    GL::RenderbufferFormat format,
                                    int width, int height);
    static void FramebufferRenderbuffer(GL::FramebufferTarget target,
                                        GL::Attachment attachment,
                                        GL::RenderbufferTarget rbTarget,
                                        GLId renderbufferId);
    static void DrawBuffers(const Array<GL::Attachment> &attachments);
    static void ReadBuffer(GL::Attachment readAttachment);

    static void ReadPixels(int x, int y, int width, int height,
                           GL::ColorComp inputComp,
                           GL::DataType inputDataType,
                           void *pixels);
    static void ReadPixels(const Recti &readRect,
                           GL::ColorComp inputComp,
                           GL::DataType inputDataType,
                           void *pixels);

    static void Finish();
    static void Flush();

    template <class T>
    static bool Uniform(const String &uniformName, const T& value,
                        bool warn = true);
    static void Uniform(int location, int value);
    static void Uniform(int location, float value);
    static void Uniform(int location, bool value);
    static void Uniform(int location, const Matrix3f &value);
    static void Uniform(int location, const Matrix4f &value);
    static void Uniform(int location, const Color &value);
    static void Uniform(int location, const Vector2 &value);
    static void Uniform(int location, const Vector3 &value);
    static void Uniform(int location, const Vector4 &value);

    static void PixelStore(GL::Enum pixelStoreEnum, int n);

    static void GenerateMipMap(GL::TextureTarget textureTarget);
    static void TexImage2D(GL::TextureTarget textureTarget,
                           int textureWidth,
                           int textureHeight,
                           GL::ColorFormat textureInternalColorFormat,
                           GL::ColorComp inputDataColorComp,
                           GL::DataType inputDataType,
                           const void *data);
    static void TexParameterFilter(GL::TextureTarget textureTarget,
                                   GL::FilterMagMin filterMagMin,
                                   GL::FilterMode filterMode);
    static void TexParameterWrap(GL::TextureTarget textureTarget,
                                 GL::WrapCoord wrapCoord,
                                 GL::WrapMode wrapMode);
    static void ActiveTexture(int activeTexture);
    static void LineWidth(float lineWidth);

    static void GenFramebuffers(int n, GLId *glIds);
    static void GenRenderBuffers(int n, GLId *glIds);
    static void GenTextures(int n, GLId *glIds);
    static void GenVertexArrays(int n, GLId *glIds);
    static void GenBuffers(int n, GLId *glIds);
    static void DeleteFramebuffers(int n, const GLId *glIds);
    static void DeleteRenderBuffers(int n, const GLId *glIds);
    static void DeleteTextures(int n, const GLId *glIds);
    static void DeleteVertexArrays(int n, const GLId *glIds);
    static void DeleteBuffers(int n, const GLId *glIds);

    static void SetViewport(const Rect &viewportNDC);
    static void SetViewport(const Recti &viewport);
    static void SetViewport(int x, int y, int width, int height);

    static void BufferData(GL::BindTarget target,
                           int dataSize, const void *data,
                           GL::UsageHint usageHint);
    static void SetColorMask(bool maskR, bool maskG, bool maskB, bool maskA);
    static void SetViewProjMode(ViewProjMode mode);
    static void SetStencilOp(GL::StencilOperation zPass);
    static void SetStencilOp(GL::StencilOperation fail,
                             GL::StencilOperation zFail,
                             GL::StencilOperation zPass);
    static void SetStencilFunc(GL::Function stencilFunction,
                               uint mask = 0xFF);
    static void SetStencilFunc(GL::Function stencilFunction,
                               Byte stencilValue,
                               uint mask = 0xFF);
    static void SetStencilValue(Byte value);
    static void SetDepthMask(bool writeDepth);
    static void SetDepthFunc(GL::Function testDepth);
    static void SetWireframe(bool wireframe);
    static void SetCullFace(const GL::Face cullFace);
    static void SetModelMatrix(const Matrix4 &model);
    static void SetViewMatrix(const Matrix4 &view);
    static void SetProjectionMatrix(const Matrix4 &projection);
    static void SetZNearFar(float zNear, float zFar);

    static Vector2  FromPixelsPointToPixelPerfect(const Vector2& ndcPoint);
    static Rect     FromGlobalNDCToPixels(const Rect &rectNDC);
    static Rect     FromPixelsToGlobalNDC(const Rect &rectPixels);
    static Vector2  FromPixelsAmountToGlobalNDC(const Vector2 &pixelsAmount);
    static Vector2  FromGlobalNDCToPixelsAmount(const Vector2 &ndcAmount);
    static Vector2  FromPixelsPointToGlobalNDC(const Vector2 &pixelsPoint);
    static Vector2  FromPixelsPointToGlobalNDC(const Vector2i &pixelsPoint);
    static Vector2  FromGlobalNDCToPixelsPoint(const Vector2 &ndcPoint);

    static void Render(const VAO* vao,
                       GL::Primitives renderMode,
                       int elementsCount,
                       int startElementIndex = 0);

    static int GetInteger(GL::Enum glEnum);
    static void GetInteger(GL::Enum glEnum, int *values);
    static bool GetBoolean(GL::Enum glEnum);
    static void GetBoolean(GL::Enum glEnum, bool *values);

    template <class T>
    static T GetUniform(GLId program, int uniformLocation);

    template <class T>
    static T GetUniform(GLId program, const String &uniformName);

    template <class T>
    static T GetUniform(const String &uniformName);

    template <class T>
    static GLSLVar<T> GetUniformAt(GLId shaderProgramId, GLuint uniformIndex);

    static int GetUniformLocation(GLId programId, const String &uniformName);

    static int GetUniformsListSize(GLId shaderProgramId);
    static DataType GetUniformTypeAt(GLId shaderProgramId, GLuint uniformIndex);

    static String GetProgramLinkErrorMsg(GLId programId);
    static int    GetProgramInteger(GLId programId, GL::Enum glEnum);
    static void   GetProgramIntegers(GLId programId, GL::Enum glEnum, GLint *ints);

    static int  GetShaderInteger(GLId shaderId, GL::Enum glEnum);
    static String GetShaderErrorMsg(GLId shaderId);

    static Recti GetViewportRect();
    static Vector2i GetViewportSize();
    static float GetViewportAspectRatio();
    static Vector2 GetViewportPixelSize();

    static Color GetClearColor();
    static GL::Enum GetPolygonMode(GL::Face face);
    static uint GetLineWidth();
    static uint GetStencilMask();
    static GL::Function GetStencilFunc();
    static GL::StencilOperation GetStencilOp();
    static Byte GetStencilValue();
    static bool IsColorMaskR();
    static bool IsColorMaskG();
    static bool IsColorMaskB();
    static bool IsColorMaskA();
    static std::array<bool, 4> GetColorMask();
    static bool GetDepthMask();
    static GL::Function GetDepthFunc();
    static bool IsWireframe();
    static GL::Face GetCullFace();

    static void GetTexImage(GL::TextureTarget textureTarget,
                            Byte *pixels);
    static void GetTexImage(GL::TextureTarget textureTarget,
                            float *pixels);
    static void GetTexImage(GL::TextureTarget textureTarget,
                            GL::DataType dataType,
                            void *pixels);

    static void Bind(const GLObject *bindable);
    static void Bind(BindTarget bindTarget, GLId glId);
    static void UnBind(const GLObject *bindable);
    static void UnBind(BindTarget bindTarget);
    static GLId GetBoundId(GL::BindTarget bindTarget);
    static bool IsBound(const GLObject *bindable);
    static bool IsBound(BindTarget bindTarget, GLId glId);

    static uint GetPixelBytesSize(GL::ColorFormat format);
    static uint GetPixelBytesSize(GL::ColorComp colorComp,
                                  GL::DataType dataType);
    static uint GetBytesSize(GL::DataType dataType);
    static uint GetNumComponents(GL::ColorComp colorComp);
    static uint GetNumComponents(GL::ColorFormat colorFormat);
    static GL::DataType GetDataTypeFrom(GL::ColorFormat format);
    static GL::ColorComp GetColorCompFrom(GL::ColorFormat format);

    static void BindUniformBufferToShader(const String &uniformBlockName,
                                          const ShaderProgram *sp,
                                          const IUniformBuffer *buffer);

    static GL::ViewProjMode GetViewProjMode();

    static GL* GetActive();
    GLUniforms *GetGLUniforms() const;

    GL();

private:
    // Context
    GLId m_boundVAOId              = 0;
    GLId m_boundVBOId              = 0;
    GLId m_boundTextureId          = 0;
    GLId m_boundFramebufferId      = 0;
    GLId m_boundShaderProgramId    = 0;
    GLId m_boundUniformBufferId    = 0;
    std::array<bool, 4> m_colorMask = {{true, true, true, true}};
    uint m_lineWidth     = 0;
    Byte m_stencilValue  = 0;
    uint m_stencilMask   = 0xFF;
    Recti m_viewportRect = Recti::Zero;

    bool m_depthMask = true;
    GL::Function m_depthFunc = GL::Function::Less;
    Color m_clearColor = Color::Zero;
    GL::Face m_cullFace = GL::Face::None;

    GL::Enum m_frontPolygonMode      = GL_FILL;
    GL::Enum m_backPolygonMode       = GL_FILL;
    GL::Enum m_frontBackPolygonMode  = GL_FILL;
    GL::Function m_stencilFunc       = GL::Function::Always;
    GL::StencilOperation m_stencilOp = GL::StencilOperation::Keep;

    GLUniforms *m_glUniforms = nullptr;

    static GL* s_activeGL;
    static void SetActive(GL *gl);

    friend class GEngine;
};

NAMESPACE_BANG_END

#include "GL.tcc"

#endif // GL_H

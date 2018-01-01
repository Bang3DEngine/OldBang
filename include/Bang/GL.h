#ifndef GL_H
#define GL_H

#include "Bang/Map.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/GLEnums.h"

NAMESPACE_BANG_BEGIN

#ifdef DEBUG
#define GL_CALL( CALL ) \
        GL_ClearError(); \
        CALL; \
        GL_CheckError()
#define GL_ClearError() GL::ClearError()
#define GL_CheckError() \
    ASSERT_SOFT_MSG( GL::CheckError(__LINE__, __FUNCTION__, __FILE__), \
                     "There was an OpenGL error, see previous message.");
#else
#define GL_CALL( CALL ) CALL
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

    static void Clear(GL_BufferBit bufferBit);
    static void ClearColorBuffer(const Color& clearColor = Color::Zero,
                                 bool clearR = true, bool clearG = true,
                                 bool clearB = true, bool clearA = true);
    static void ClearDepthBuffer(float clearDepth = 1.0f);
    static void ClearStencilBuffer(int stencilValue = 0);

    static void Enablei (GL_Test glTest, int index);
    static void Disablei(GL_Test glTest, int index);
    static void Enable (GL_Test glTest);
    static void Disable(GL_Test glTest);
    static bool IsEnabled(GL_Test glTest);
    static bool IsEnabledi(GL_Test glTest, int index);


    static void EnableVertexAttribArray(int location);
    static void DisableVertexAttribArray(int location);
    static void VertexAttribPointer(int location,
                                    int dataComponentsCount,
                                    GL_DataType dataType,
                                    bool dataNormalized,
                                    int dataStride,
                                    int dataOffset);

    static void PolygonMode(GL_Face face, GL_Enum mode);

    static GLvoid* MapBuffer(GL_BindTarget target, GL_Enum access);
    static void UnMapBuffer(GL_BindTarget target);

    static void BlendFunc(GL_BlendFactor srcFactor, GL_BlendFactor dstFactor);

    static void BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1,
                                int dstX0, int dstY0, int dstX1, int dstY1,
                                GL_FilterMode filterMode,
                                GL_BufferBit bufferBitMask);
    static void BlitFramebuffer(const Recti &srcRect, const Recti &dstRect,
                                GL_FilterMode filterMode,
                                GL_BufferBit bufferBitMask);

    static GLId CreateShader(GL_ShaderType shaderType);
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

    static void FramebufferTexture2D(GL_FramebufferTarget target,
                                     GL_Attachment attachment,
                                     GL_TextureTarget texTarget,
                                     GLId textureId);
    static void BindRenderbuffer(GL_RenderbufferTarget target,
                                 GLId renderbufferId);
    static void RenderbufferStorage(GL_RenderbufferTarget target,
                                    GL_RenderbufferFormat format,
                                    int width, int height);
    static void FramebufferRenderbuffer(GL_FramebufferTarget target,
                                        GL_Attachment attachment,
                                        GL_RenderbufferTarget rbTarget,
                                        GLId renderbufferId);
    static void DrawBuffers(const Array<GL_Attachment> &attachments);
    static void ReadBuffer(GL_Attachment readAttachment);

    static void ReadPixels(int x, int y, int width, int height,
                           GL_ColorComp inputComp,
                           GL_DataType outputDataType,
                           void *pixels);
    static void ReadPixels(const Recti &readRect,
                           GL_ColorComp inputComp,
                           GL_DataType outputDataType,
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

    static void PixelStore(GL_Enum pixelStoreEnum, int n);

    static void GenerateMipMap(GL_TextureTarget textureTarget);
    static void TexImage2D(GL_TextureTarget textureTarget,
                           int textureWidth,
                           int textureHeight,
                           GL_ColorFormat textureInternalColorFormat,
                           GL_ColorComp inputDataColorComp,
                           GL_DataType inputDataType,
                           const void *data);
    static void TexParameterFilter(GL_TextureTarget textureTarget,
                                   GL_FilterMagMin filterMagMin,
                                   GL_FilterMode filterMode);
    static void TexParameterWrap(GL_TextureTarget textureTarget,
                                 GL_WrapCoord wrapCoord,
                                 GL_WrapMode wrapMode);
    static void ActiveTexture(int activeTexture);
    static void LineWidth(float lineWidth);
    static void PointSize(float pointSize);

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

    static void BufferData(GL_BindTarget target,
                           int dataSize, const void *data,
                           GL_UsageHint usageHint);
    static void SetColorMask(bool maskR, bool maskG, bool maskB, bool maskA);
    static void SetViewProjMode(GL_ViewProjMode mode);
    static void SetStencilOp(GL_StencilOperation zPass);
    static void SetStencilOp(GL_StencilOperation fail,
                             GL_StencilOperation zFail,
                             GL_StencilOperation zPass);
    static void SetStencilFunc(GL_Function stencilFunction,
                               uint mask = 0xFF);
    static void SetStencilFunc(GL_Function stencilFunction,
                               Byte stencilValue,
                               uint mask = 0xFF);
    static void SetStencilValue(Byte value);
    static void SetDepthMask(bool writeDepth);
    static void SetDepthFunc(GL_Function testDepth);
    static void SetWireframe(bool wireframe);
    static void SetCullFace(const GL_Face cullFace);
    static void SetModelMatrix(const Matrix4 &model);
    static void SetViewMatrix(const Matrix4 &view);
    static void SetProjectionMatrix(const Matrix4 &projection);
    static void SetZNearFar(float zNear, float zFar);

    static Vector2  FromPixelsPointToPixelPerfect(const Vector2& screenPoint);
    static Vector2  FromPointToPointNDC(const Vector2 &point,
                                        const Vector2 &rectSize);
    static Vector2  FromPointNDCToPoint(const Vector2 &pointNDC,
                                        const Vector2 &rectSize);
    static Vector2  FromAmountToAmountNDC(const Vector2 &amountNDC,
                                          const Vector2 &rectSize);
    static Vector2  FromAmountNDCToAmount(const Vector2 &amountNDC,
                                          const Vector2 &rectSize);
    static Vector2  FromScreenPointToViewportPoint(const Vector2& screenPoint,
                                                   const Recti& viewport);
    static Vector2  FromViewportPointToScreenPoint(const Vector2& vpPoint);
    static Vector2  FromViewportPointToScreenPoint(const Vector2i& vpPoint);
    static Vector2  FromScreenPointToViewportPoint(const Vector2& screenPoint);
    static Vector2  FromScreenPointToViewportPoint(const Vector2i& screenPoint);
    static Rect     FromViewportRectToViewportRectNDC(const Rect &vpRect);
    static Rect     FromViewportRectNDCToViewportRect(const Rect &vpRectNDC);
    static Rect     FromScreenRectToScreenRectNDC(const Rect &screenRect);
    static Rect     FromScreenRectNDCToScreenRect(const Rect &screenRectNDC);
    static Vector2  FromViewportAmountToViewportAmountNDC(const Vector2 &vpAmount);
    static Vector2  FromViewportAmountNDCToViewportAmount(const Vector2 &vpAmountNDC);
    static Vector2  FromScreenPointNDCToScreenPoint(const Vector2  &screenPointNDC);
    static Vector2  FromScreenPointToScreenPointNDC(const Vector2  &screenPoint);
    static Vector2  FromScreenPointToScreenPointNDC(const Vector2i &screenPoint);
    static Vector2  FromViewportPointToViewportPointNDC(const Vector2  &vpPoint);
    static Vector2  FromViewportPointToViewportPointNDC(const Vector2i &vpPoint);
    static Vector2  FromViewportPointNDCToViewportPoint(const Vector2  &vpPoint);

    static void Render(const VAO* vao,
                       GL_Primitive renderMode,
                       int elementsCount,
                       int startElementIndex = 0);

    static int GetInteger(GL_Enum glEnum);
    static void GetInteger(GL_Enum glEnum, int *values);
    static bool GetBoolean(GL_Enum glEnum);
    static void GetBoolean(GL_Enum glEnum, bool *values);

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
    static GL_DataType GetUniformTypeAt(GLId shaderProgramId, GLuint uniformIndex);

    static String GetProgramLinkErrorMsg(GLId programId);
    static int    GetProgramInteger(GLId programId, GL_Enum glEnum);
    static void   GetProgramIntegers(GLId programId, GL_Enum glEnum, GLint *ints);

    static int  GetShaderInteger(GLId shaderId, GL_Enum glEnum);
    static String GetShaderErrorMsg(GLId shaderId);

    static Recti GetViewportRect();
    static Vector2i GetViewportSize();
    static float GetViewportAspectRatio();
    static Vector2 GetViewportPixelSize();

    static Color GetClearColor();
    static GL_Enum GetPolygonMode(GL_Face face);
    static uint GetLineWidth();
    static uint GetStencilMask();
    static GL_Function GetStencilFunc();
    static GL_StencilOperation GetStencilOp();
    static Byte GetStencilValue();
    static bool IsColorMaskR();
    static bool IsColorMaskG();
    static bool IsColorMaskB();
    static bool IsColorMaskA();
    static std::array<bool, 4> GetColorMask();
    static bool GetDepthMask();
    static GL_Function GetDepthFunc();
    static bool IsWireframe();
    static GL_Face GetCullFace();

    static void GetTexImage(GL_TextureTarget textureTarget,
                            Byte *pixels);
    static void GetTexImage(GL_TextureTarget textureTarget,
                            float *pixels);
    static void GetTexImage(GL_TextureTarget textureTarget,
                            GL_DataType dataType,
                            void *pixels);

    static void Bind(const GLObject *bindable);
    static void Bind(GL_BindTarget bindTarget, GLId glId);
    static void UnBind(const GLObject *bindable);
    static void UnBind(GL_BindTarget bindTarget);
    static GLId GetBoundId(GL_BindTarget bindTarget);
    static bool IsBound(const GLObject *bindable);
    static bool IsBound(GL_BindTarget bindTarget, GLId glId);

    static uint GetPixelBytesSize(GL_ColorFormat format);
    static uint GetPixelBytesSize(GL_ColorComp colorComp,
                                  GL_DataType dataType);
    static uint GetBytesSize(GL_DataType dataType);
    static uint GetNumComponents(GL_ColorComp colorComp);
    static uint GetNumComponents(GL_ColorFormat colorFormat);
    static GL_DataType GetDataTypeFrom(GL_ColorFormat format);
    static GL_ColorComp GetColorCompFrom(GL_ColorFormat format);

    static void BindUniformBufferToShader(const String &uniformBlockName,
                                          const ShaderProgram *sp,
                                          const IUniformBuffer *buffer);

    static GL_ViewProjMode GetViewProjMode();

    static GL* GetActive();
    GLUniforms *GetGLUniforms() const;

    GL();
    virtual ~GL();

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
    Map<GL_Test, bool> m_enabledTests;
    Map<std::pair<GL_Test, int>, bool> m_enabled_i_Tests;

    bool m_depthMask = true;
    GL_Function m_depthFunc = GL_Function::Less;
    Color m_clearColor = Color::Zero;
    GL_Face m_cullFace = GL_Face::Back;

    GL_Enum m_frontPolygonMode      = GL_Enum::Fill;
    GL_Enum m_backPolygonMode       = GL_Enum::Fill;
    GL_Enum m_frontBackPolygonMode  = GL_Enum::Fill;
    GL_Function m_stencilFunc       = GL_Function::Always;
    GL_StencilOperation m_stencilOp = GL_StencilOperation::Keep;

    GLUniforms *m_glUniforms = nullptr;

    static GL* s_activeGL;
    static void SetActive(GL *gl);

    friend class GEngine;
};

NAMESPACE_BANG_END

#include "GL.tcc"

#endif // GL_H

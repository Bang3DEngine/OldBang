#include "Bang/GL.h"

#include "Bang/VAO.h"
#include "Bang/Debug.h"
#include "Bang/Matrix3.h"
#include "Bang/GEngine.h"
#include "Bang/Texture.h"
#include "Bang/GLObject.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

void GL::ClearError()
{
    glGetError();
}

bool GL::CheckError(int line, const String &func, const String &file)
{
    GLenum glError;
    bool ok = true;
    glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        const char *err = reinterpret_cast<const char*>(gluErrorString(glError));
        Debug_Error("OpenGL error: " << err << " At function " << func << " in "
                    << file << ":" << line);
        ok = false;
    }
    return ok;
}

bool GL::CheckFramebufferError()
{
    bool error = false;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        String errMsg = "There was a framebuffer error.";
        Debug_Error(errMsg);
        GL_CheckError();
        error = true;
    }
    return error;
}

void GL::Clear(GL::BufferBit bufferBit)
{
    glClear( GLCAST(bufferBit) );
}

void GL::ClearColorBuffer(const Color &clearColor,
                          bool clearR, bool clearG, bool clearB, bool clearA)
{
    bool colorMaskRBefore = GL::IsColorMaskR();
    bool colorMaskGBefore = GL::IsColorMaskG();
    bool colorMaskBBefore = GL::IsColorMaskB();
    bool colorMaskABefore = GL::IsColorMaskA();
    GL::SetColorMask(clearR, clearG, clearB, clearA);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    GL::Clear(GL::BufferBit::Color);

    GL::SetColorMask(colorMaskRBefore, colorMaskGBefore,
                     colorMaskBBefore, colorMaskABefore);
}

void GL::ClearDepthBuffer(float clearDepth)
{
    glClearDepth(clearDepth);
    GL::Clear(GL::BufferBit::Depth);
}

void GL::ClearStencilBuffer(int stencilValue)
{
    glClearStencil(stencilValue);
    GL::Clear(GL::BufferBit::Stencil);
}

void GL::EnableVertexAttribArray(int location)
{
    GL::ClearError();
    glEnableVertexAttribArray(location);
    GL_CheckError();
}

void GL::DisableVertexAttribArray(int location)
{
    GL::ClearError();
    glDisableVertexAttribArray(location);
    GL_CheckError();
}

void GL::VertexAttribPointer(int location,
                             int dataComponentsCount,
                             GL::DataType dataType,
                             bool dataNormalized,
                             int dataStride,
                             int dataOffset)
{
    GL::ClearError();
    glVertexAttribPointer(location,
                          dataComponentsCount,
                          GLCAST(dataType),
                          dataNormalized,
                          dataStride,
                          RCAST<void*>(dataOffset));
    GL_CheckError();
}

void GL::BlendFunc(GL::BlendFactor srcFactor, GL::BlendFactor dstFactor)
{
    ENSURE(srcFactor != GL::BlendFactor::None &&
           dstFactor != GL::BlendFactor::None);

    GL::ClearError();
    glBlendFunc( GLCAST(srcFactor), GLCAST(dstFactor) );
    GL_CheckError();
}

void GL::Enable(GL::Enum glEnum)
{
    glEnable(glEnum);
}

void GL::Disable(GL::Enum glEnum)
{
    glDisable(glEnum);
}

void GL::BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1,
                         int dstX0, int dstY0, int dstX1, int dstY1,
                         GL::FilterMode filterMode,
                         GL::BufferBit bufferBitMask)
{
    GL::ClearError();
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1,
                      dstX0, dstY0, dstX1, dstY1,
                      GLCAST(bufferBitMask),
                      GLCAST(filterMode));
    GL_CheckError();
}

void GL::BlitFramebuffer(const Recti &srcRect, const Recti &dstRect,
                         GL::FilterMode filterMode,
                         GL::BufferBit bufferBitMask)
{
    GL::BlitFramebuffer(srcRect.GetMin().x, srcRect.GetMin().y,
                        srcRect.GetMax().x, srcRect.GetMax().y,
                        dstRect.GetMin().x, dstRect.GetMin().y,
                        dstRect.GetMax().x, dstRect.GetMax().y,
                        filterMode, bufferBitMask);
}

GLId GL::CreateShader(GL::ShaderType shaderType)
{
    GL::ClearError();
    GLId id = glCreateShader( GLCAST(shaderType) );
    GL_CheckError();
    return id;
}

void GL::ShaderSource(GLId shaderId, const String &sourceCode)
{
    GL::ClearError();
    int sourceSize = sourceCode.Size();
    const char *src = sourceCode.ToCString();
    glShaderSource(shaderId, 1, &src, &sourceSize);
    GL_CheckError();
}

bool GL::CompileShader(GLId shaderId)
{
    GL::ClearError();
    glCompileShader(shaderId);
    GL_CheckError();
    bool ok = GL::GetShaderInteger(shaderId, GL_COMPILE_STATUS);
    GL_CheckError();
    return ok;
}

int GL::GetShaderInteger(GLId shaderId, GL::Enum glEnum)
{
    int v = false;
    GL::ClearError();
    glGetShaderiv(shaderId, glEnum, &v);
    GL_CheckError();
    return v;
}

String GL::GetShaderErrorMsg(GLId shaderId)
{
    int maxLength = GL::GetShaderInteger(shaderId, GL_INFO_LOG_LENGTH);

    GL::ClearError();
    Array<char> v(maxLength);
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, &v[0]);
    GL_CheckError();

    return String(v.begin(), v.end());
}

void GL::DeleteShader(GLId shaderId)
{
    GL::ClearError();
    glDeleteShader(shaderId);
    GL_CheckError();
}

GLId GL::CreateProgram()
{
    return glCreateProgram();
}

void GL::AttachShader(GLId programId, GLId shaderId)
{
    GL::ClearError();
    glAttachShader(programId, shaderId);
    GL_CheckError();
}

bool GL::LinkProgram(GLId programId)
{
    GL::ClearError();
    glLinkProgram(programId);
    GL_CheckError();
    int linked = GL::GetProgramInteger(programId, GL_LINK_STATUS);
    return linked;
}

String GL::GetProgramLinkErrorMsg(GLId programId)
{
    GLint errorLength = GL::GetProgramInteger(programId, GL_INFO_LOG_LENGTH);
    if (errorLength > 1)
    {
       char* errorLog = new char[errorLength];
       glGetProgramInfoLog(programId, errorLength, NULL, errorLog);
       String error = String(errorLog);
       delete[] errorLog;
       return error;
    }
    return String();
}

int GL::GetProgramInteger(GLId programId, GL::Enum glEnum)
{
    int result = 0;
    GL::ClearError();
    glGetProgramiv(programId, glEnum, &result);
    GL_CheckError();
    return result;
}

void GL::BindAttribLocation(GLId programId, int location,
                            const String &attribName)
{
    GL::ClearError();
    glBindAttribLocation(programId, location, attribName.ToCString());
    GL_CheckError();
}

void GL::BindFragDataLocation(GLId programId, int location,
                              const String &fragDataName)
{
    GL::ClearError();
    glBindFragDataLocation(programId, location, fragDataName.ToCString());
    GL_CheckError();
}

int GL::GetUniformLocation(GLId programId, const String &uniformName)
{
    GL::ClearError();
    int location = glGetUniformLocation(programId, uniformName.ToCString());
    GL_CheckError();
    return location;
}

void GL::DeleteProgram(GLId programId)
{
    GL::ClearError();
    glDeleteProgram(programId);
    GL_CheckError();
}

void GL::FramebufferTexture2D(GL::FramebufferTarget target,
                              GL::Attachment attachment,
                              GL::TextureTarget texTarget,
                              GLId textureId)
{
    GL::ClearError();
    glFramebufferTexture2D(GLCAST(target),
                           GLCAST(attachment),
                           GLCAST(texTarget),
                           textureId,
                           0);
    GL_CheckError();
}

void GL::BindRenderbuffer(GL::RenderbufferTarget target, GLId renderbufferId)
{
    GL::ClearError();
    glBindRenderbuffer(GLCAST(target), renderbufferId);
    GL_CheckError();
}

void GL::RenderbufferStorage(GL::RenderbufferTarget target,
                             GL::RenderbufferFormat format,
                             int width, int height)
{
    GL::ClearError();
    glRenderbufferStorage(GLCAST(target),
                          GLCAST(format),
                          width, height);
    GL_CheckError();
}

void GL::FramebufferRenderbuffer(GL::FramebufferTarget target,
                                 GL::Attachment attachment,
                                 GL::RenderbufferTarget rbTarget,
                                 GLId renderbufferId)
{
    GL::ClearError();
    glFramebufferRenderbuffer(GLCAST(target),
                              GLCAST(attachment),
                              GLCAST(rbTarget),
                              renderbufferId);
    GL_CheckError();
}

void GL::DrawBuffers(const Array<GL::Attachment> &attachments)
{
    GL::ClearError();
    glDrawBuffers(attachments.Size(), (const GLenum*)(&attachments[0]));
    GL_CheckError();
}

void GL::ReadBuffer(GL::Attachment readAttachment)
{
    GL::ClearError();
    glReadBuffer( GLCAST(readAttachment) );
    GL_CheckError();
}

void GL::ReadPixels(int x, int y, int width, int height,
                    GL::ColorComp inputComp, GL::DataType inputDataType,
                    void *pixels)
{
    GL::ClearError();
    glReadPixels(x, y, width, height,
                 GLCAST(inputComp),
                 GLCAST(inputDataType),
                 pixels);
    GL_CheckError();
}

void GL::ReadPixels(const Recti &readRect, GL::ColorComp inputComp,
                    GL::DataType inputDataType, void *pixels)
{
    GL::ReadPixels(readRect.GetMin().x, readRect.GetMin().y,
                   readRect.GetWidth(), readRect.GetHeight(),
                   inputComp, inputDataType, pixels);
}

void GL::Finish() { glFinish(); }
void GL::Flush() { glFlush(); }

void GL::Uniform(int location, int value)
{
    glUniform1i(location, value);
}
void GL::Uniform(int location, float value)
{
    glUniform1f(location, value);
}
void GL::Uniform(int location, bool value)
{
    glUniform1i(location, value ? 1 : 0);
}
void GL::Uniform(int location, const Matrix3f& value)
{
    glUniformMatrix3fv(location, 1, false, value.Data());
}
void GL::Uniform(int location, const Matrix4f& value)
{
    glUniformMatrix4fv(location, 1, false, value.Data());
}
void GL::Uniform(int location, const Color &value)
{
    GL::Uniform(location, value.ToVector4());
}
void GL::Uniform(int location, const Vector2 &value)
{
    glUniform2fv(location, 1, value.Data());
}
void GL::Uniform(int location, const Vector3 &value)
{
    glUniform3fv(location, 1, value.Data());
}
void GL::Uniform(int location, const Vector4 &value)
{
    glUniform4fv(location, 1, value.Data());
}

void GL::PixelStore(GL::Enum pixelStoreEnum, int n)
{
    glPixelStorei(pixelStoreEnum, n);
}

void GL::GenerateMipMap(GL::TextureTarget textureTarget)
{
    GL::ClearError();
    glGenerateMipmap( GLCAST(textureTarget) );
    GL_CheckError();
}

void GL::TexImage2D(GL::TextureTarget textureTarget,
                    int textureWidth,
                    int textureHeight,
                    GL::ColorFormat textureInternalColorFormat,
                    GL::ColorComp inputDataColorComp,
                    GL::DataType inputDataType,
                    const void *data)
{
    GL::ClearError();

    glTexImage2D(GLCAST(textureTarget),
                 0,
                 GLCAST(textureInternalColorFormat),
                 textureWidth,
                 textureHeight,
                 0,
                 GLCAST(inputDataColorComp),
                 GLCAST(inputDataType),
                 data);

    GL_CheckError();
}

void GL::TexParameterFilter(GL::TextureTarget textureTarget,
                            GL::FilterMagMin filterMagMin,
                            GL::FilterMode filterMode)
{
    glTexParameteri(GLCAST(textureTarget),
                    GLCAST(filterMagMin),
                    GLCAST(filterMode));
}

void GL::TexParameterWrap(GL::TextureTarget textureTarget,
                          GL::WrapCoord wrapCoord,
                          GL::WrapMode wrapMode)
{
    glTexParameteri(GLCAST(textureTarget),
                    GLCAST(wrapCoord),
                    GLCAST(wrapMode));
}

void GL::GetTexImage(GL::TextureTarget textureTarget,
                     Byte *pixels)
{
    GL::GetTexImage(textureTarget, GL::DataType::UnsignedByte, pixels);
}

void GL::GetTexImage(GL::TextureTarget textureTarget, float *pixels)
{
    GL::GetTexImage(textureTarget, GL::DataType::Float, pixels);
}

void GL::GetTexImage(GL::TextureTarget textureTarget,
                     GL::DataType dataType,
                     void *pixels)
{
    GL::ClearError();

    glGetTexImage(GLCAST(textureTarget),
                  0,
                  GLCAST(GL::ColorComp::RGBA),
                  GLCAST(dataType),
                  SCAST<void*>(pixels));

    GL_CheckError();
}

bool GL::GetBoolean(GL::Enum glEnum)
{
    bool result;
    GL::GetBoolean(glEnum, &result);
    return result;
}
void GL::GetBoolean(GL::Enum glEnum, bool *values)
{
    GLboolean result;
    glGetBooleanv(GLCAST(glEnum), &result);
    *values = result;
}

int GL::GetInteger(GL::Enum glEnum)
{
    int result;
    GL::GetInteger(glEnum, &result);
    return result;
}
void GL::GetInteger(GL::Enum glEnum, int *values)
{
    glGetIntegerv(glEnum, values);
}

void GL::ActiveTexture(int activeTexture)
{
    ASSERT(activeTexture >= GL_TEXTURE0);
    glActiveTexture(activeTexture);
}

void GL::LineWidth(float lineWidth)
{
    glLineWidth(lineWidth);
}

void GL::GenFramebuffers(int n, GLId *glIds)
{
    glGenFramebuffers(n, glIds);
}

void GL::GenRenderBuffers(int n, GLId *glIds)
{
    glGenRenderbuffers(n, glIds);
}

void GL::GenTextures(int n, GLId *glIds)
{
    glGenTextures(n, glIds);
}

void GL::GenVertexArrays(int n, GLId *glIds)
{
    glGenVertexArrays(n, glIds);
}

void GL::GenBuffers(int n, GLId *glIds)
{
    glGenBuffers(n, glIds);
}

void GL::DeleteFramebuffers(int n, const GLId *glIds)
{
    glDeleteFramebuffers(n, glIds);
}

void GL::DeleteRenderBuffers(int n, const GLId *glIds)
{
    glDeleteRenderbuffers(n, glIds);
}

void GL::DeleteTextures(int n, const GLId *glIds)
{
    glDeleteTextures(n, glIds);
}

void GL::DeleteVertexArrays(int n, const GLId *glIds)
{
    glDeleteVertexArrays(n, glIds);
}

void GL::DeleteBuffers(int n, const GLId *glIds)
{
    glDeleteBuffers(n, glIds);
}

void GL::SetViewport(const Recti &viewport)
{
    GL::SetViewport(viewport.GetMin().x, viewport.GetMin().y,
                    viewport.GetWidth(), viewport.GetHeight());
}

void GL::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void GL::SetLineWidth(float lineWidth)
{
    glLineWidth(lineWidth);
}

Recti GL::GetViewportRect()
{
    int viewport[4];
    GL::GetInteger(GL::Viewport, viewport);
    return Recti( viewport[0], viewport[1],
                 (viewport[0] + viewport[2]),
                 (viewport[1] + viewport[3]));
}

Vector2i GL::GetViewportSize()
{
    return GL::GetViewportRect().GetSize();
}

float GL::GetViewportAspectRatio()
{
    Vector2i vpSize = GL::GetViewportSize();
    return SCAST<float>(vpSize.x) / vpSize.y;
}

Vector2 GL::GetViewportPixelSize()
{
    return 1.0f / Vector2(GL::GetViewportSize());
}

void GL::BufferDataVBO(int dataSize, const void *data, GL::UsageHint usageHint)
{
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GLCAST(usageHint));
}

void GL::Render(const VAO *vao, GL::Primitives primitivesMode,
                int elementsCount, int startIndex)
{
    vao->Bind();
    glDrawArrays( GLCAST(primitivesMode), startIndex, elementsCount);
    vao->UnBind();
}

uint GL::GetStencilMask()
{
    return SCAST<uint>(GL::GetInteger(GL_STENCIL_VALUE_MASK));
}

GL::Function GL::GetStencilFunc()
{
    return SCAST<GL::Function>(GL::GetInteger(GL_STENCIL_FUNC));
}

void GL::Bind(const GLObject *bindable)
{
    GL::Bind(bindable->GetGLBindTarget(), bindable->GetGLId());
}

void GL::Bind(GL::BindTarget bindTarget, GLId glId)
{
    switch (bindTarget)
    {
        case BindTarget::Texture2D:
            glBindTexture(GLCAST(GL::TextureTarget::Texture2D), glId);
        break;
        case BindTarget::ShaderProgram:
            glUseProgram(glId);
        break;
        case BindTarget::Framebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, glId);
        break;
        case BindTarget::VAO:
            glBindVertexArray(glId);
        break;
        case BindTarget::VBO:
            glBindBuffer(GL_ARRAY_BUFFER, glId);
        break;
        default:
        break;
    }
}

void GL::UnBind(const GLObject *bindable)
{
    GL::UnBind(bindable->GetGLBindTarget());
}

void GL::UnBind(GL::BindTarget bindTarget)
{
    GL::Bind(bindTarget, 0);
}

bool GL::IsBound(const GLObject *bindable)
{
    return GL::IsBound(bindable->GetGLBindTarget(), bindable->GetGLId());
}

void GL::ApplyToShaderProgram(ShaderProgram *sp)
{
    ENSURE(sp); ASSERT(GL::IsBound(sp));
    const Matrix4& modelMatrix = GL::GetModelMatrix();
    sp->Set("B_Model",    modelMatrix);
    sp->Set("B_ModelInv", modelMatrix.Inversed());

    Matrix3 normalMatrix = Matrix3(modelMatrix.c0.xyz(),
                                   modelMatrix.c1.xyz(),
                                   modelMatrix.c2.xyz()
                               ).Transposed().Inversed();
    sp->Set("B_Normal",    normalMatrix);
    sp->Set("B_NormalInv", normalMatrix.Inversed());

    const Matrix4& viewMatrix = GL::GetViewMatrix();
    sp->Set("B_View",    viewMatrix);
    sp->Set("B_ViewInv", viewMatrix.Inversed());

    const Matrix4& projectionMatrix = GL::GetProjectionMatrix();
    sp->Set("B_Projection",    projectionMatrix);
    sp->Set("B_ProjectionInv", projectionMatrix.Inversed());

    GL *gl = GL::GetActive();
    sp->Set("B_Camera_Near", gl->m_zNear);
    sp->Set("B_Camera_Far",  gl->m_zFar);

    sp->Set("B_ViewportMinPos", Vector2(GL::GetViewportRect().GetMin()));
    sp->Set("B_ViewportSize",   Vector2(GL::GetViewportSize()));

    Matrix4 pvmMatrix;
    if (gl->m_viewProjMode == GL::ViewProjMode::UseBoth)
    {
        pvmMatrix = projectionMatrix * viewMatrix * modelMatrix;
    }
    else if (gl->m_viewProjMode == GL::ViewProjMode::OnlyFixAspectRatio)
    {
        Matrix4 modelTranslate( Vector4(1,0,0,0),
                                Vector4(0,1,0,0),
                                Vector4(0,0,1,0),
                                modelMatrix.c3);

        Matrix4 modelNoTranslate = modelMatrix;
        modelNoTranslate.SetTranslate( Vector3(0,0,0) );

        float ar = 1.0f / GL::GetViewportAspectRatio();
        Matrix4 fixAR(ar, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);

        pvmMatrix = modelTranslate * fixAR * modelNoTranslate;
    }
    else if (gl->m_viewProjMode == GL::ViewProjMode::IgnoreBoth)
    {
        pvmMatrix = modelMatrix;
    }

    sp->Set("B_PVM", pvmMatrix);
}

void GL::SetColorMask(bool maskR, bool maskG, bool maskB, bool maskA)
{
    glColorMask(maskR, maskG, maskB, maskA);
}

void GL::SetViewProjMode(GL::ViewProjMode mode)
{
    GL *gl = GL::GetActive();
    gl->m_viewProjMode = mode;
}

void GL::SetStencilOp(GL::StencilOperation fail,
                      GL::StencilOperation zFail,
                      GL::StencilOperation zPass)
{
    glStencilOp(GLCAST(fail), GLCAST(zFail), GLCAST(zPass));
}

void GL::SetStencilFunc(GL::Function stencilFunction, uint mask)
{
    GL::SetStencilFunc(stencilFunction, GL::GetStencilValue(), mask);
}

void GL::SetStencilFunc(GL::Function stencilFunction,
                        Byte stencilValue,
                        uint mask)
{
    glStencilFunc(GLCAST(stencilFunction), stencilValue, mask);
}

void GL::SetStencilOp(GL::StencilOperation zPass)
{
    GL::SetStencilOp(GL::StencilOperation::Keep,
                     GL::StencilOperation::Keep,
                     zPass);
}

void GL::SetStencilValue(Byte value)
{
    GL::SetStencilFunc(GetStencilFunc(), value, GetStencilMask());
}

void GL::SetDepthMask(bool writeDepth)
{
    glDepthMask(writeDepth);
}

void GL::SetDepthFunc(GL::Function depthFunc)
{
    glDepthFunc( GLCAST(depthFunc) );
}

void GL::SetWireframe(bool wireframe)
{
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void GL::SetCullFace(GL::Face cullFace)
{
    glCullFace( GLCAST(cullFace) );
}

void GL::SetModelMatrix(const Matrix4 &model)
{
    GL::GetActive()->m_modelMatrix = model;
}

void GL::SetViewMatrix(const Matrix4 &view)
{
    GL::GetActive()->m_viewMatrix = view;
}

void GL::SetProjectionMatrix(const Matrix4 &projection)
{
    GL::GetActive()->m_projectionMatrix = projection;
}

void GL::SetZNearFar(float zNear, float zFar)
{
    GL *gl = GL::GetActive();
    gl->m_zNear = zNear;
    gl->m_zFar  = zFar;
}

Recti GL::FromGlobalNDCToPixels(const Rect &rectNDC)
{
    return Recti( GL::FromGlobalNDCToPixelsPoint(rectNDC.GetMin()),
                  GL::FromGlobalNDCToPixelsPoint(rectNDC.GetMax()) );
}

Rect GL::FromPixelsToGlobalNDC(const Recti &rectPixels)
{
    return Rect( GL::FromPixelsPointToGlobalNDC(rectPixels.GetMin()),
                 GL::FromPixelsPointToGlobalNDC(rectPixels.GetMax()) );
}

Vector2 GL::FromPixelsAmountToGlobalNDC(const Vector2i &pixels)
{
    return (Vector2f(pixels) / Vector2f(GL::GetViewportSize())) * 2.0f;
}

Vector2i GL::FromGlobalNDCToPixelsAmount(const Vector2 &ndcAmount)
{
    return Vector2i(ndcAmount * Vector2f(GL::GetViewportSize()) * 0.5f);
}

Vector2 GL::FromPixelsPointToGlobalNDC(const Vector2 &_pixelsPoint)
{
    Vector2i screenSize( GL::GetViewportSize() );
    Vector2 pixelsPoint(_pixelsPoint.x, (screenSize.y-1) - _pixelsPoint.y);
    Vector2 res = ( Vector2f(pixelsPoint) / Vector2f(screenSize) ) * 2.0f - 1.0f;
    return res;
}
Vector2 GL::FromPixelsPointToGlobalNDC(const Vector2i &pixelsPoint)
{
    return GL::FromPixelsPointToGlobalNDC( Vector2f(pixelsPoint) );
}

Vector2i GL::FromGlobalNDCToPixelsPoint(const Vector2 &_ndcPoint)
{
    Vector2 ndcPoint(_ndcPoint.x,  _ndcPoint.y);
    Vector2 screenSize( GL::GetViewportSize() );
    Vector2i resultPx( Vector2::Round( (ndcPoint * 0.5f + 0.5f) * screenSize) );
    return Vector2i(resultPx.x, (screenSize.y-1) - resultPx.y);
}




GL::StencilOperation GL::GetStencilOp()
{
    return SCAST<GL::StencilOperation>(
                        GL::GetInteger(GL_STENCIL_PASS_DEPTH_PASS));
}
Byte GL::GetStencilValue()
{
    return SCAST<Byte>(GL::GetInteger(GL_STENCIL_REF));
}

Array<BoolByte> GL::GetColorMask()
{
    int colorMask[4];
    GL::GetInteger(GL_COLOR_WRITEMASK, &colorMask[0]);
    return {SCAST<BoolByte>(colorMask[0]),
            SCAST<BoolByte>(colorMask[1]),
            SCAST<BoolByte>(colorMask[2]),
            SCAST<BoolByte>(colorMask[3])};
}
bool GL::IsColorMaskR()  { return GL::GetColorMask().At(0) == 1;  }
bool GL::IsColorMaskG()  { return GL::GetColorMask().At(1) == 1;  }
bool GL::IsColorMaskB()  { return GL::GetColorMask().At(2) == 1;  }
bool GL::IsColorMaskA()  { return GL::GetColorMask().At(3) == 1;  }

bool GL::GetDepthMask()  {  return GL::GetBoolean(GL_DEPTH_WRITEMASK);  }
GL::Function GL::GetDepthFunc()
{
    return SCAST<GL::Function>(GL::GetInteger(GL_DEPTH_FUNC));
}

bool GL::IsWireframe()
{
    return GL::GetInteger(GL_POLYGON_MODE) == GL_LINE;
}
GL::Face GL::GetCullFace()
{
    return SCAST<GL::Face>(GL::GetInteger(GL_CULL_FACE_MODE));
}

const Matrix4& GL::GetModelMatrix()
{ return GL::GetActive()->m_modelMatrix; }

const Matrix4& GL::GetViewMatrix()
{ return GL::GetActive()->m_viewMatrix; }

const Matrix4& GL::GetProjectionMatrix()
{ return GL::GetActive()->m_projectionMatrix; }

GLId GL::GetBoundId(GL::BindTarget bindTarget)
{
    switch(bindTarget)
    {
        case GL::BindTarget::Texture2D:
            return GL::GetInteger(GL_TEXTURE_BINDING_2D);
        case GL::BindTarget::Framebuffer:
            return GL::GetInteger(GL_FRAMEBUFFER_BINDING);
        case GL::BindTarget::VAO:
            return GL::GetInteger(GL_VERTEX_ARRAY_BINDING);
        case GL::BindTarget::VBO:
            return GL::GetInteger(GL_BUFFER_BINDING);
        case GL::BindTarget::ShaderProgram:
            return GL::GetInteger(GL_CURRENT_PROGRAM);
        default: return 0;
    }
    return 0;
}

bool GL::IsBound(GL::BindTarget bindTarget, GLId glId)
{
    return GL::GetBoundId(bindTarget) == glId;
}

uint GL::GetPixelBytesSize(GL::ColorFormat texFormat)
{
    switch (texFormat)
    {
        case GL::ColorFormat::RGBA_UByte8:      return 8;
        case GL::ColorFormat::RGBA_Float16:     return 16;
        case GL::ColorFormat::Depth24_Stencil8: return 32;
        case GL::ColorFormat::RGB10_A2_UByte:   return 32;
        case GL::ColorFormat::RGBA_Float32:     return 128;
    }
    return 0;
}

uint GL::GetPixelBytesSize(GL::ColorComp colorComp, GL::DataType dataType)
{
    return GL::GetNumComponents(colorComp) * GL::GetBytesSize(dataType);
}

uint GL::GetBytesSize(GL::DataType dataType)
{
    if (dataType == GL::DataType::Byte)   { return sizeof(Byte); }
    if (dataType == GL::DataType::UnsignedByte) { return sizeof(Byte); }
    if (dataType == GL::DataType::Short) { return sizeof(short); }
    if (dataType == GL::DataType::UnsignedShort) { return sizeof(unsigned short); }
    if (dataType == GL::DataType::Int) { return sizeof(int); }
    if (dataType == GL::DataType::UnsignedInt) { return sizeof(uint); }
    if (dataType == GL::DataType::Float)  { return sizeof(float); }
    if (dataType == GL::DataType::Double) { return sizeof(double); }
    return 1;
}

uint GL::GetNumComponents(GL::ColorComp colorComp)
{
    if (colorComp == GL::ColorComp::RGBA)          { return 4; }
    if (colorComp == GL::ColorComp::RGB)           { return 3; }
    if (colorComp == GL::ColorComp::DepthStencil)  { return 2; }
    ASSERT(false);
    return 3;
}

uint GL::GetNumComponents(GL::ColorFormat colorFormat)
{
    return GL::GetNumComponents( GL::GetColorCompFrom(colorFormat) );
}

GL::DataType GL::GetDataTypeFrom(GL::ColorFormat format)
{
    if (format == GL::ColorFormat::RGBA_UByte8)
    {
        return GL::DataType::UnsignedByte;
    }
    else if (format == GL::ColorFormat::RGBA_Float16 ||
             format == GL::ColorFormat::RGBA_Float32)
    {
        return GL::DataType::Float;
    }
    else if (format == GL::ColorFormat::Depth24_Stencil8)
    {
        return GL::DataType::Float;
    }
    ASSERT(false);
    return GL::DataType::Float;
}

GL::ColorComp GL::GetColorCompFrom(GL::ColorFormat format)
{
    if (format == GL::ColorFormat::RGBA_UByte8   ||
        format == GL::ColorFormat::RGBA_Float16 ||
        format == GL::ColorFormat::RGBA_Float32)
    {
        return GL::ColorComp::RGBA;
    }
    else if (format == GL::ColorFormat::Depth24_Stencil8)
    {
        return GL::ColorComp::DepthStencil;
    }
    ASSERT(false);
    return GL::ColorComp::RGB;
}

GL *GL::GetActive()
{
    GEngine *gp = GEngine::GetInstance();
    return gp ? gp->GetGL() : nullptr;
}

GL::GL() {}

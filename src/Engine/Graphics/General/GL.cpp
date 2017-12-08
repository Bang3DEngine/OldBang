#include "Bang/GL.h"

#include "Bang/VAO.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Matrix3.h"
#include "Bang/GEngine.h"
#include "Bang/Texture.h"
#include "Bang/GLObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

GL* GL::s_activeGL = nullptr;

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

Color GL::GetClearColor()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_clearColor : Color::Zero;
}

void GL::Clear(GL::BufferBit bufferBit)
{
    glClear( GLCAST(bufferBit) );
}

void GL::ClearColorBuffer(const Color &clearColor,
                          bool clearR, bool clearG, bool clearB, bool clearA)
{
    std::array<bool, 4> lastColorMask = GL::GetColorMask();
    bool differentColorMask =
                (lastColorMask[0] != clearR) || (lastColorMask[1] != clearG) ||
                (lastColorMask[2] != clearB) || (lastColorMask[3] != clearA);

    if (differentColorMask)
    { GL::SetColorMask(clearR, clearG, clearB, clearA); }

    if (GL::GetClearColor() != clearColor)
    {
        GL::GetActive()->m_clearColor = clearColor;
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }

    GL::Clear(GL::BufferBit::Color);

    if (differentColorMask)
    {
        GL::SetColorMask(lastColorMask[0], lastColorMask[1],
                         lastColorMask[2], lastColorMask[3]);
    }
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
    GL_ClearError();
    glEnableVertexAttribArray(location);
    GL_CheckError();
}

void GL::DisableVertexAttribArray(int location)
{
    GL_ClearError();
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
    GL_ClearError();
    glVertexAttribPointer(location,
                          dataComponentsCount,
                          GLCAST(dataType),
                          dataNormalized,
                          dataStride,
                          RCAST<void*>(dataOffset));
    GL_CheckError();
}

void GL::PolygonMode(GL::Face face, GL::Enum mode)
{
    if (GL::GetPolygonMode(face) != mode)
    {
        switch (face)
        {
            case GL::Face::FrontAndBack:
                GL::GetActive()->m_frontBackPolygonMode = mode; break;
            case GL::Face::Front:
                GL::GetActive()->m_frontPolygonMode = mode; break;
            case GL::Face::Back:
                GL::GetActive()->m_backPolygonMode = mode; break;
            default: return;
        }

        glPolygonMode(GLCAST(face), mode);
    }
}

GL::Enum GL::GetPolygonMode(GL::Face face)
{
    switch (face)
    {
        case GL::Face::FrontAndBack: return GL::GetActive()->m_frontBackPolygonMode;
        case GL::Face::Front: return GL::GetActive()->m_frontPolygonMode;
        case GL::Face::Back: return GL::GetActive()->m_backPolygonMode;
        default: break;
    }
    return  GL_FILL;
}

GLvoid* GL::MapBuffer(GL::BindTarget target, GL::Enum access)
{
    return glMapBuffer(GLCAST(target), access);
}

void GL::UnMapBuffer(GL::BindTarget target)
{
    glUnmapBuffer( GLCAST(target) );
}

int GL::GetUniformsListSize(GLId shaderProgramId)
{
    return GL::GetProgramInteger(shaderProgramId, GL_ACTIVE_UNIFORMS);
}

GL::DataType GL::GetUniformTypeAt(GLId shaderProgramId, GLuint uniformIndex)
{
    if (shaderProgramId == 0) { return DataType::Byte; }

    GLint size;
    GL::Enum type;
    GLsizei length;
    constexpr GLsizei bufSize = 128;
    GLchar cname[bufSize];

    glGetActiveUniform(shaderProgramId,
                       Cast<GLuint>(uniformIndex),
                       bufSize, &length,
                       &size, &type, cname);

    return Cast<GL::DataType>(type);
}

void GL::BlendFunc(GL::BlendFactor srcFactor, GL::BlendFactor dstFactor)
{
    ENSURE(srcFactor != GL::BlendFactor::None &&
           dstFactor != GL::BlendFactor::None);

    GL_ClearError();
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
    GL_ClearError();
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
    GL_ClearError();
    GLId id = glCreateShader( GLCAST(shaderType) );
    GL_CheckError();
    return id;
}

void GL::ShaderSource(GLId shaderId, const String &sourceCode)
{
    GL_ClearError();
    int sourceSize = sourceCode.Size();
    const char *src = sourceCode.ToCString();
    glShaderSource(shaderId, 1, &src, &sourceSize);
    GL_CheckError();
}

bool GL::CompileShader(GLId shaderId)
{
    GL_ClearError();
    glCompileShader(shaderId);
    GL_CheckError();
    bool ok = GL::GetShaderInteger(shaderId, GL_COMPILE_STATUS);
    GL_CheckError();
    return ok;
}

int GL::GetShaderInteger(GLId shaderId, GL::Enum glEnum)
{
    int v = false;
    GL_ClearError();
    glGetShaderiv(shaderId, glEnum, &v);
    GL_CheckError();
    return v;
}

String GL::GetShaderErrorMsg(GLId shaderId)
{
    int maxLength = GL::GetShaderInteger(shaderId, GL_INFO_LOG_LENGTH);

    GL_ClearError();
    Array<char> v(maxLength);
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, &v[0]);
    GL_CheckError();

    return String(v.begin(), v.end());
}

void GL::DeleteShader(GLId shaderId)
{
    GL_ClearError();
    glDeleteShader(shaderId);
    GL_CheckError();
}

GLId GL::CreateProgram()
{
    return glCreateProgram();
}

void GL::AttachShader(GLId programId, GLId shaderId)
{
    GL_ClearError();
    glAttachShader(programId, shaderId);
    GL_CheckError();
}

bool GL::LinkProgram(GLId programId)
{
    GL_ClearError();
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
    GL::GetProgramIntegers(programId, glEnum, &result);
    return result;
}

void GL::GetProgramIntegers(GLId programId, GL::Enum glEnum, GLint *ints)
{
    GL_ClearError();
    glGetProgramiv(programId, glEnum, ints);
    GL_CheckError();
}

void GL::BindAttribLocation(GLId programId, int location,
                            const String &attribName)
{
    GL_ClearError();
    glBindAttribLocation(programId, location, attribName.ToCString());
    GL_CheckError();
}

void GL::BindFragDataLocation(GLId programId, int location,
                              const String &fragDataName)
{
    GL_ClearError();
    glBindFragDataLocation(programId, location, fragDataName.ToCString());
    GL_CheckError();
}

int GL::GetUniformLocation(GLId programId, const String &uniformName)
{
    GL_ClearError();
    int location = glGetUniformLocation(programId, uniformName.ToCString());
    GL_CheckError();
    return location;
}

void GL::DeleteProgram(GLId programId)
{
    GL_ClearError();
    glDeleteProgram(programId);
    GL_CheckError();
}

void GL::FramebufferTexture2D(GL::FramebufferTarget target,
                              GL::Attachment attachment,
                              GL::TextureTarget texTarget,
                              GLId textureId)
{
    GL_ClearError();
    glFramebufferTexture2D(GLCAST(target),
                           GLCAST(attachment),
                           GLCAST(texTarget),
                           textureId,
                           0);
    GL_CheckError();
}

void GL::BindRenderbuffer(GL::RenderbufferTarget target, GLId renderbufferId)
{
    GL_ClearError();
    glBindRenderbuffer(GLCAST(target), renderbufferId);
    GL_CheckError();
}

void GL::RenderbufferStorage(GL::RenderbufferTarget target,
                             GL::RenderbufferFormat format,
                             int width, int height)
{
    GL_ClearError();
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
    GL_ClearError();
    glFramebufferRenderbuffer(GLCAST(target),
                              GLCAST(attachment),
                              GLCAST(rbTarget),
                              renderbufferId);
    GL_CheckError();
}

void GL::DrawBuffers(const Array<GL::Attachment> &attachments)
{
    GL_ClearError();
    glDrawBuffers(attachments.Size(), (const GLenum*)(&attachments[0]));
    GL_CheckError();
}

void GL::ReadBuffer(GL::Attachment readAttachment)
{
    GL_ClearError();
    glReadBuffer( GLCAST(readAttachment) );
    GL_CheckError();
}

void GL::ReadPixels(int x, int y, int width, int height,
                    GL::ColorComp inputComp, GL::DataType inputDataType,
                    void *pixels)
{
    GL_ClearError();
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
    GL_ClearError();
    glUniform1i(location, value);
    GL_CheckError();
}
void GL::Uniform(int location, float value)
{
    GL_ClearError();
    glUniform1f(location, value);
    GL_CheckError();
}
void GL::Uniform(int location, bool value)
{
    GL_ClearError();
    glUniform1i(location, value ? 1 : 0);
    GL_CheckError();
}
void GL::Uniform(int location, const Matrix3f& value)
{
    GL_ClearError();
    glUniformMatrix3fv(location, 1, false, value.Data());
    GL_CheckError();
}
void GL::Uniform(int location, const Matrix4f& value)
{
    GL_ClearError();
    glUniformMatrix4fv(location, 1, false, value.Data());
    GL_CheckError();
}
void GL::Uniform(int location, const Color &value)
{
    GL::Uniform(location, value.ToVector4());
}
void GL::Uniform(int location, const Vector2 &value)
{
    GL_ClearError();
    glUniform2fv(location, 1, value.Data());
    GL_CheckError();
}
void GL::Uniform(int location, const Vector3 &value)
{
    GL_ClearError();
    glUniform3fv(location, 1, value.Data());
    GL_CheckError();
}
void GL::Uniform(int location, const Vector4 &value)
{
    GL_ClearError();
    glUniform4fv(location, 1, value.Data());
    GL_CheckError();
}

void GL::PixelStore(GL::Enum pixelStoreEnum, int n)
{
    GL_ClearError();
    glPixelStorei(pixelStoreEnum, n);
    GL_CheckError();
}

void GL::GenerateMipMap(GL::TextureTarget textureTarget)
{
    GL_ClearError();
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
    GL_ClearError();

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
    GL_ClearError();

    glGetTexImage(GLCAST(textureTarget),
                  0,
                  GLCAST(GL::ColorComp::RGBA),
                  GLCAST(dataType),
                  Cast<void*>(pixels));

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
    GL_ClearError();
    GLboolean result;
    glGetBooleanv(GLCAST(glEnum), &result);
    *values = result;
    GL_CheckError();
}

int GL::GetInteger(GL::Enum glEnum)
{
    GLint result;
    GL::GetInteger(glEnum, &result);
    return result;
}
void GL::GetInteger(GL::Enum glEnum, int *values)
{
    GL_ClearError();
    glGetIntegerv(glEnum, values);
    GL_CheckError();
}

void GL::ActiveTexture(int activeTexture)
{
    ASSERT(activeTexture >= GL_TEXTURE0);
    glActiveTexture(activeTexture);
}

void GL::LineWidth(float lineWidth)
{
    if (GL::GetLineWidth() != lineWidth)
    {
        GL::GetActive()->m_lineWidth = lineWidth;
        glLineWidth(lineWidth);
    }
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

void GL::SetViewport(const Rect &viewportNDC)
{
    Vector2 minPx = GL::FromGlobalNDCToPixelsPoint(viewportNDC.GetMin());
    Vector2 maxPx = GL::FromGlobalNDCToPixelsAmount(viewportNDC.GetMax());
    GL::SetViewport( Recti(minPx.x, minPx.y, maxPx.x, maxPx.y) );
}

void GL::SetViewport(const Recti &viewport)
{
    GL::SetViewport(viewport.GetMin().x, viewport.GetMin().y,
                    viewport.GetWidth(), viewport.GetHeight());
}

void GL::SetViewport(int x, int y, int width, int height)
{
    Recti vpRect( Vector2i(x, y), Vector2i(x+width, y+height));
    if (GL::GetViewportRect() != vpRect)
    {
        GL *gl = GL::GetActive();
        if (gl) { gl->m_viewportRect = vpRect; }
        glViewport(x, y, width, height);

        if (gl && GLUniforms::GetActive())
        {
            auto *vpBuffer = GLUniforms::GetViewportBuffer();
            vpBuffer->GetData()->minPos = Vector2(GL::GetViewportRect().GetMin());
            vpBuffer->GetData()->size   = Vector2(GL::GetViewportSize());
            vpBuffer->UpdateBuffer();
        }
    }
}

Recti GL::GetViewportRect()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_viewportRect : Recti::Zero;
}

Vector2i GL::GetViewportSize()
{
    return GL::GetViewportRect().GetSize();
}

float GL::GetViewportAspectRatio()
{
    Vector2i vpSize = GL::GetViewportSize();
    return Cast<float>(vpSize.x) / vpSize.y;
}

Vector2 GL::GetViewportPixelSize()
{
    return 1.0f / Vector2(GL::GetViewportSize());
}

void GL::BufferData(GL::BindTarget target, int dataSize,
                    const void *data, GL::UsageHint usageHint)
{
    glBufferData(GLCAST(target), dataSize, data, GLCAST(usageHint));
}

void GL::Render(const VAO *vao, GL::Primitives primitivesMode,
                int elementsCount, int startIndex)
{
    vao->Bind();
    glDrawArrays( GLCAST(primitivesMode), startIndex, elementsCount);
    vao->UnBind();
}

uint GL::GetLineWidth()
{
    return GL::GetActive()->m_lineWidth;
}

uint GL::GetStencilMask()
{
    return GL::GetActive()->m_stencilMask;
}

GL::Function GL::GetStencilFunc()
{
    return GL::GetActive()->m_stencilFunc;
}

void GL::Bind(const GLObject *bindable)
{
    GL::Bind(bindable->GetGLBindTarget(), bindable->GetGLId());
}

void GL::Bind(GL::BindTarget bindTarget, GLId glId)
{
    GL *gl = GL::GetActive();

    GL_ClearError();
    switch (bindTarget)
    {
        case BindTarget::Texture2D:
            if (gl) { gl->m_boundTextureId = glId; }
            glBindTexture( GLCAST(GL::BindTarget::Texture2D), glId);
        break;
        case BindTarget::ShaderProgram:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundShaderProgramId = glId; }
            glUseProgram(glId);
        break;
        case BindTarget::Framebuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundFramebufferId = glId; }
            glBindFramebuffer( GLCAST(GL::BindTarget::Framebuffer), glId);
        break;
        case BindTarget::VAO:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if(gl) { gl->m_boundVAOId = glId; }
            glBindVertexArray(glId);
        break;
        case BindTarget::VBO:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundVBOId = glId; }
            glBindBuffer( GLCAST(GL::BindTarget::VBO), glId);
        break;
        case BindTarget::UniformBuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundUniformBufferId = glId; }
            glBindBuffer( GLCAST(GL::BindTarget::UniformBuffer), glId);
        break;

        default: ASSERT(false); break;
    }
    GL_CheckError();
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

void GL::SetColorMask(bool maskR, bool maskG, bool maskB, bool maskA)
{
    GL *gl = GL::GetActive();
    std::array<bool,4> newColorMask = {{maskR, maskG, maskB, maskA}};
    if (!gl || (GL::GetColorMask() != newColorMask))
    {
        gl->m_colorMask = newColorMask;
        glColorMask(maskR, maskG, maskB, maskA);
    }
}

void GL::SetViewProjMode(GL::ViewProjMode mode)
{
    GL::GetActive()->GetGLUniforms()->SetViewProjMode(mode);
}

void GL::SetStencilOp(GL::StencilOperation fail,
                      GL::StencilOperation zFail,
                      GL::StencilOperation zPass)
{
    GL::GetActive()->m_stencilOp = zPass;
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
    if (stencilFunction != GL::GetStencilFunc() ||
        stencilValue != GL::GetStencilValue() ||
        mask != GL::GetStencilMask())
    {
        GL *gl = GL::GetActive();
        gl->m_stencilFunc  = stencilFunction;
        gl->m_stencilValue = stencilValue;
        gl->m_stencilMask  = mask;
        glStencilFunc(GLCAST(stencilFunction), stencilValue, mask);
    }
}

void GL::SetStencilOp(GL::StencilOperation zPass)
{
    if (GL::GetStencilOp() != zPass)
    {
        GL::SetStencilOp(GL::StencilOperation::Keep,
                         GL::StencilOperation::Keep,
                         zPass);
    }
}

void GL::SetStencilValue(Byte value)
{
    GL::SetStencilFunc(GetStencilFunc(), value, GetStencilMask());
}

void GL::SetDepthMask(bool writeDepth)
{
    if (GL::GetActive()->GetDepthMask() != writeDepth)
    {
        GL *gl = GL::GetActive();
        gl->m_depthMask = writeDepth;
        glDepthMask(writeDepth);
    }
}

void GL::SetDepthFunc(GL::Function depthFunc)
{
    if (GL::GetDepthFunc() != depthFunc)
    {
        GL::GetActive()->m_depthFunc = depthFunc;
        glDepthFunc( GLCAST(depthFunc) );
    }
}

void GL::SetWireframe(bool wireframe)
{
    if (GL::IsWireframe() != wireframe)
    {
        GL::PolygonMode(GL::Face::FrontAndBack, wireframe ? GL_LINE : GL_FILL);
    }
}

Vector2 GL::FromPixelsPointToPixelPerfect(const Vector2 &ndcPoint)
{
    return Vector2::Floor(ndcPoint);
}

void GL::SetCullFace(GL::Face cullFace)
{
    if (GL::GetCullFace() != cullFace)
    {
        GL::GetActive()->m_cullFace = cullFace;
        glCullFace( GLCAST(cullFace) );
    }
}

Rect GL::FromGlobalNDCToPixels(const Rect &rectNDC)
{
    Vector2 min = GL::FromGlobalNDCToPixelsPoint(rectNDC.GetMin());
    Vector2 max = GL::FromGlobalNDCToPixelsPoint(rectNDC.GetMax());
    return Rect(min, max);
}

Rect GL::FromPixelsToGlobalNDC(const Rect &rectPixels)
{
    Vector2 min  = GL::FromPixelsPointToGlobalNDC(rectPixels.GetMin());
    Vector2 size = GL::FromPixelsAmountToGlobalNDC(rectPixels.GetSize());
    return Rect(min, (min+size));
}

Vector2 GL::FromPixelsAmountToGlobalNDC(const Vector2 &pixels)
{
    return (pixels / Vector2(GL::GetViewportSize())) * 2.0f;
}

Vector2 GL::FromGlobalNDCToPixelsAmount(const Vector2 &ndcAmount)
{
    return Vector2::Round((ndcAmount * Vector2(GL::GetViewportSize())) * 0.5f);
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
    return GL::FromPixelsPointToGlobalNDC(pixelsPoint);
}

Vector2 GL::FromGlobalNDCToPixelsPoint(const Vector2 &_ndcPoint)
{
    Vector2 ndcPoint(_ndcPoint.x,  _ndcPoint.y);
    Vector2 screenSize( GL::GetViewportSize() );
    Vector2 resultPx ( Vector2::Round((ndcPoint * 0.5f + 0.5f) * screenSize) );
    return Vector2(resultPx.x, (screenSize.y-1) - resultPx.y);
}




GL::StencilOperation GL::GetStencilOp()
{
    return GL::GetActive()->m_stencilOp;
}
Byte GL::GetStencilValue()
{
    return GL::GetActive()->m_stencilValue;
}

std::array<bool, 4> GL::GetColorMask()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_colorMask : std::array<bool, 4>({{true, true, true, true}});
}
bool GL::IsColorMaskR()  { return GL::GetColorMask()[0];  }
bool GL::IsColorMaskG()  { return GL::GetColorMask()[1];  }
bool GL::IsColorMaskB()  { return GL::GetColorMask()[2];  }
bool GL::IsColorMaskA()  { return GL::GetColorMask()[3];  }

bool GL::GetDepthMask()  { return GL::GetActive()->m_depthMask; }
GL::Function GL::GetDepthFunc() { return GL::GetActive()->m_depthFunc; }

bool GL::IsWireframe()
{
    return GL::GetPolygonMode(GL::Face::FrontAndBack) == GL_LINE;
}
GL::Face GL::GetCullFace()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_cullFace : GL::Face::None;
}

GLId GL::GetBoundId(GL::BindTarget bindTarget)
{
    switch(bindTarget)
    {
        case GL::BindTarget::Texture2D:
            return GL::GetActive()->m_boundTextureId;
        case GL::BindTarget::Framebuffer:
            return GL::GetActive()->m_boundFramebufferId;
        case GL::BindTarget::VAO:
            return GL::GetActive()->m_boundVAOId;
        case GL::BindTarget::VBO:
            return GL::GetActive()->m_boundVBOId;
        case GL::BindTarget::ShaderProgram:
            return GL::GetActive()->m_boundShaderProgramId;
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
    if (format == GL::ColorFormat::RGBA_UByte8  ||
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

void GL::BindUniformBufferToShader(const String &uniformBlockName,
                                   const ShaderProgram *sp,
                                   const IUniformBuffer *buffer)
{
    GLuint blockIndex = glGetUniformBlockIndex(sp->GetGLId(),
                                               uniformBlockName.ToCString());
    glUniformBlockBinding(sp->GetGLId(), blockIndex, buffer->GetBindingPoint());
}

GL::ViewProjMode GL::GetViewProjMode()
{
    return GL::GetActive()->GetGLUniforms()->GetViewProjMode();
}

GL *GL::GetActive()
{
    return GL::s_activeGL;
}

GLUniforms *GL::GetGLUniforms() const { return m_glUniforms; }

GL::GL()
{
    m_glUniforms = new GLUniforms();
}

GL::~GL()
{
    delete m_glUniforms;
}

void GL::SetActive(GL *gl)
{
    GL::s_activeGL = gl;
}

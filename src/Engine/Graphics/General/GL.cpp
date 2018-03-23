#include "Bang/GL.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include "Bang/VAO.h"
#include "Bang/List.h"
#include "Bang/Rect.h"
#include "Bang/Debug.h"
#include "Bang/Window.h"
#include "Bang/GEngine.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/Texture.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/GLObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

GL* GL::s_activeGL = nullptr;

GL::GL()
{
    m_glUniforms = new GLUniforms();
    GL::Enable(GL::Test::Depth);
    GL::Enable(GL::Test::Stencil);
    GL::Enable(GL::Test::CullFace);
}

GL::~GL()
{
    delete m_glUniforms;
}

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
        Debug_Error("OpenGL error \"" << String(err).ToUpper() <<
                    "\" at function \"" << func << "\" in " <<
                    file <<
                    ":" <<
                    line);
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

const Color& GL::GetClearColor()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_clearColor : Color::Zero;
}

void GL::Clear(GL::BufferBit bufferBit)
{
    GL_CALL( glClear( GLCAST(bufferBit) ) );
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
        GL_CALL( glClearColor(clearColor.r, clearColor.g,
                              clearColor.b, clearColor.a) );
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
    GL_CALL( glClearDepth(clearDepth) );
    GL::Clear(GL::BufferBit::Depth);
}

void GL::ClearStencilBuffer(int stencilValue)
{
    GL_CALL( glClearStencil(stencilValue) );
    GL::Clear(GL::BufferBit::Stencil);
}

void GL::EnableVertexAttribArray(int location)
{
    GL_CALL( glEnableVertexAttribArray(location) );
}

void GL::DisableVertexAttribArray(int location)
{
    GL_CALL( glDisableVertexAttribArray(location) );
}

void GL::VertexAttribPointer(int location,
                             int dataComponentsCount,
                             GL::DataType dataType,
                             bool dataNormalized,
                             int dataStride,
                             int dataOffset)
{
    GL_CALL(
    glVertexAttribPointer(location,
                          dataComponentsCount,
                          GLCAST(dataType),
                          dataNormalized,
                          dataStride,
                          RCAST<void*>(dataOffset))
    );
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
    return  GL::Fill;
}

GLvoid* GL::MapBuffer(GL::BindTarget target, GL::Enum access)
{
    GL_CALL( GLvoid *ret = glMapBuffer(GLCAST(target), access) );
    return ret;
}

void GL::UnMapBuffer(GL::BindTarget target)
{
    GL_CALL( glUnmapBuffer( GLCAST(target) ) );
}

int GL::GetUniformsListSize(GLId shaderProgramId)
{
    return GL::GetProgramInteger(shaderProgramId, GL::ActiveUniforms);
}

GL::DataType GL::GetUniformTypeAt(GLId shaderProgramId, GLuint uniformIndex)
{
    if (shaderProgramId == 0) { return GL::DataType::Byte; }

    GLint size;
    GLenum type;
    GLsizei length;
    constexpr GLsizei bufSize = 128;
    GLchar cname[bufSize];

    GL_CALL(
    glGetActiveUniform(shaderProgramId,
                       Cast<GLuint>(uniformIndex),
                       bufSize,
                       &length,
                       &size,
                       &type,
                       cname)
    );

    return Cast<GL::DataType>(type);
}

void GL::BlendFunc(GL::BlendFactor srcFactor, GL::BlendFactor dstFactor)
{
    GL::BlendFuncSeparate(srcFactor, dstFactor, srcFactor, dstFactor);
}

void GL::BlendFuncSeparate(GL::BlendFactor srcFactorColor,
                           GL::BlendFactor dstFactorColor,
                           GL::BlendFactor srcFactorAlpha,
                           GL::BlendFactor dstFactorAlpha)
{
    if (srcFactorColor != GL::GetBlendSrcFactorColor() ||
        dstFactorColor != GL::GetBlendDstFactorColor() ||
        srcFactorAlpha != GL::GetBlendSrcFactorAlpha() ||
        dstFactorAlpha != GL::GetBlendDstFactorAlpha())
    {
        GL::GetActive()->m_blendSrcFactorColor = srcFactorColor;
        GL::GetActive()->m_blendDstFactorColor = dstFactorColor;
        GL::GetActive()->m_blendSrcFactorAlpha = srcFactorAlpha;
        GL::GetActive()->m_blendDstFactorAlpha = dstFactorAlpha;
        GL_CALL( glBlendFuncSeparate( GLCAST(srcFactorColor),
                                      GLCAST(dstFactorColor),
                                      GLCAST(srcFactorAlpha),
                                      GLCAST(dstFactorAlpha)) );
    }
}

void GL::BlendEquation(GL::BlendEquationE blendEquation)
{
    GL::BlendEquationSeparate(blendEquation, blendEquation);
}

void GL::BlendEquationSeparate(GL::BlendEquationE blendEquationColor,
                               GL::BlendEquationE blendEquationAlpha)
{
    if (blendEquationColor != GL::GetBlendEquationColor() ||
        blendEquationAlpha != GL::GetBlendEquationAlpha() )
    {
        GL::GetActive()->m_blendEquationColor = blendEquationColor;
        GL::GetActive()->m_blendEquationAlpha = blendEquationAlpha;
        GL_CALL( glBlendEquationSeparate( GLCAST(blendEquationColor),
                                          GLCAST(blendEquationAlpha) ) );
    }
}

void GL::Enablei (GL::Test glTest, int i)
{
    if (!GL::IsEnabledi(glTest, i))
    {
        GL_CALL( glEnablei( GLCAST(glTest), i ) );

        GL *gl = GL::GetActive();
        if (gl) { gl->m_enabled_i_Tests[std::make_pair(glTest, i)] = true; }
    }
}
void GL::Disablei(GL::Test glTest, int i)
{
    if (GL::IsEnabledi(glTest, i))
    {
        GL_CALL( glDisablei( GLCAST(glTest), i ) );

        GL *gl = GL::GetActive();
        if (gl) { gl->m_enabled_i_Tests[std::make_pair(glTest, i)] = false; }
    }
}

void GL::Enable(GL::Test glTest)
{
    if (!GL::IsEnabled(glTest))
    {
        GL_CALL( glEnable( GLCAST(glTest) ) );

        GL *gl = GL::GetActive();
        if (gl) { gl->m_enabledTests[glTest] = true; }
    }
}

void GL::Disable(GL::Test glTest)
{
    if (GL::IsEnabled(glTest))
    {
        GL_CALL( glDisable( GLCAST(glTest) ) );

        GL *gl = GL::GetActive();
        if (gl) { gl->m_enabledTests[glTest] = false; }
    }
}

void GL::SetEnabled(GL::Test glTest, bool enabled)
{
    if (enabled) { GL::Enable(glTest); } else { GL::Disable(glTest); }
}

void GL::SetEnabledi(GL::Test glTest, int index, bool enabled)
{
    if (enabled) { GL::Enablei(glTest, index); }
    else { GL::Disablei(glTest, index); }
}

bool GL::IsEnabled(GL::Test glTest)
{
    GL *gl = GL::GetActive();
    if (!gl) { return false; }

    if (!gl->m_enabledTests.ContainsKey(glTest))
    {
        gl->m_enabledTests.Add(glTest, false);
    }
    return gl->m_enabledTests.Get(glTest);
}

bool GL::IsEnabledi(GL::Test glTest, int index)
{
    GL *gl = GL::GetActive();
    if (!gl) { return false; }

    std::pair<GL::Test, int> glTest_i = std::make_pair(glTest, index);
    if (!gl->m_enabled_i_Tests.ContainsKey(glTest_i))
    {
        gl->m_enabled_i_Tests.Add(glTest_i, false);
    }
    return gl->m_enabled_i_Tests.Get(glTest_i);
}

void GL::BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1,
                         int dstX0, int dstY0, int dstX1, int dstY1,
                         GL::FilterMode filterMode,
                         GL::BufferBit bufferBitMask)
{
    GL_CALL(
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1,
                      dstX0, dstY0, dstX1, dstY1,
                      GLCAST(bufferBitMask),
                      GLCAST(filterMode))
    );
}

void GL::BlitFramebuffer(const AARecti &srcRect, const AARecti &dstRect,
                         GL::FilterMode filterMode,
                         GL::BufferBit bufferBitMask)
{
    GL::BlitFramebuffer(srcRect.GetMin().x, srcRect.GetMin().y,
                        srcRect.GetMax().x, srcRect.GetMax().y,
                        dstRect.GetMin().x, dstRect.GetMin().y,
                        dstRect.GetMax().x, dstRect.GetMax().y,
                        filterMode, bufferBitMask);
}

void GL::Scissor(int x, int y, int width, int height)
{
    GL::Scissor( AARecti(x, y, x+width, y+height) );
}

void GL::Scissor(const AARecti &scissorRectPx)
{
    if (scissorRectPx != GL::GetScissorRect())
    {
        GL::GetActive()->m_scissorRectPx = scissorRectPx;
        GL_CALL( glScissor(scissorRectPx.GetMin().x, scissorRectPx.GetMin().y,
                           scissorRectPx.GetWidth(), scissorRectPx.GetHeight()) );
    }
}

void GL::ScissorIntersecting(int x, int y, int width, int height)
{
    GL::ScissorIntersecting( AARecti(x, y, x+width, y+height) );
}

void GL::ScissorIntersecting(const AARecti &scissorRectPx)
{
    AARecti prevScissor = GL::GetScissorRect();
    AARecti additiveScissor = AARecti::Intersection(prevScissor, scissorRectPx);
    GL::Scissor(additiveScissor);
}

GLId GL::CreateShader(GL::ShaderType shaderType)
{
    GL_CALL( GLId id = glCreateShader( GLCAST(shaderType) ) );
    return id;
}

void GL::ShaderSource(GLId shaderId, const String &sourceCode)
{
    int sourceSize = sourceCode.Size();
    const char *src = sourceCode.ToCString();
    GL_CALL( glShaderSource(shaderId, 1, &src, &sourceSize) );
}

bool GL::CompileShader(GLId shaderId)
{
    GL_ClearError();
    glCompileShader(shaderId);
    GL_CALL( bool ok = GL::GetShaderInteger(shaderId, GL::CompileStatus) );
    return ok;
}

int GL::GetShaderInteger(GLId shaderId, GL::Enum glEnum)
{
    int v = false;
    GL_CALL( glGetShaderiv(shaderId, glEnum, &v) );
    return v;
}

String GL::GetShaderErrorMsg(GLId shaderId)
{
    int maxLength = GL::GetShaderInteger(shaderId, GL::InfoLogLength);

    Array<char> v(maxLength);
    GL_CALL( glGetShaderInfoLog(shaderId, maxLength, &maxLength, &v[0]) );

    return String(v.begin(), v.end());
}

void GL::DeleteShader(GLId shaderId)
{
    GL_CALL( glDeleteShader(shaderId) );
}

GLId GL::CreateProgram()
{
    GL_CALL( GLId id = glCreateProgram() );
    return id;
}

void GL::AttachShader(GLId programId, GLId shaderId)
{
    GL_CALL( glAttachShader(programId, shaderId) );
}

bool GL::LinkProgram(GLId programId)
{
    GL_CALL( glLinkProgram(programId) );
    int linked = GL::GetProgramInteger(programId, GL::LinkStatus);
    return linked;
}

String GL::GetProgramLinkErrorMsg(GLId programId)
{
    GLint errorLength = GL::GetProgramInteger(programId, GL::InfoLogLength);
    if (errorLength > 1)
    {
       char* errorLog = new char[errorLength];
       GL_CALL( glGetProgramInfoLog(programId, errorLength, NULL, errorLog) );
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
    GL_CALL( glGetProgramiv(programId, glEnum, ints) );
}

void GL::BindAttribLocation(GLId programId, int location,
                            const String &attribName)
{
    GL_CALL( glBindAttribLocation(programId, location,
                                  attribName.ToCString()) );
}

void GL::BindFragDataLocation(GLId programId, int location,
                              const String &fragDataName)
{
    GL_CALL( glBindFragDataLocation(programId, location,
                                    fragDataName.ToCString()) );
}

int GL::GetUniformLocation(const String &uniformName)
{
    return GL::GetUniformLocation(GL::GetBoundId(GL::BindTarget::ShaderProgram),
                                  uniformName);
}
int GL::GetUniformLocation(GLId programId, const String &uniformName)
{
    GL_CALL( int location = glGetUniformLocation(programId,
                                                 uniformName.ToCString()) );
    return location;
}

void GL::DeleteProgram(GLId programId)
{
    GL_CALL( glDeleteProgram(programId) );
}

void GL::FramebufferTexture2D(GL::FramebufferTarget target,
                              GL::Attachment attachment,
                              GL::TextureTarget texTarget,
                              GLId textureId)
{
    GL_CALL(
    glFramebufferTexture2D(GLCAST(target),
                           GLCAST(attachment),
                           GLCAST(texTarget),
                           textureId,
                           0);
    );
}

void GL::BindRenderbuffer(GL::RenderbufferTarget target, GLId renderbufferId)
{
    GL_CALL( glBindRenderbuffer(GLCAST(target), renderbufferId) );
}

void GL::RenderbufferStorage(GL::RenderbufferTarget target,
                             GL::RenderbufferFormat format,
                             int width, int height)
{
    GL_CALL(
    glRenderbufferStorage(GLCAST(target),
                          GLCAST(format),
                          width, height)
    );
}

void GL::FramebufferRenderbuffer(GL::FramebufferTarget target,
                                 GL::Attachment attachment,
                                 GL::RenderbufferTarget rbTarget,
                                 GLId renderbufferId)
{
    GL_CALL(
    glFramebufferRenderbuffer(GLCAST(target),
                              GLCAST(attachment),
                              GLCAST(rbTarget),
                              renderbufferId)
    );
}

void GL::DrawBuffers(const Array<GL::Attachment> &drawAttachments)
{
    GL *gl = GL::GetActive();
    // if (!Containers::Equal(gl->m_drawBuffers.Begin(), gl->m_drawBuffers.End(),
    //                        drawAttachments.Begin()))
    {
        gl->m_drawBuffers = drawAttachments;
        GL_CALL( glDrawBuffers(drawAttachments.Size(),
                               (const GLenum*)(&drawAttachments[0])) );
    }
}

void GL::ReadBuffer(GL::Attachment readAttachment)
{
    GL *gl = GL::GetActive();
    // if (gl->m_readBuffer != readAttachment)
    {
        gl->m_readBuffer = readAttachment;
        GL_CALL( glReadBuffer( GLCAST(readAttachment) ) );
    }
}

void GL::ReadPixels(int x, int y, int width, int height,
                    GL::ColorComp inputComp, GL::DataType outputDataType,
                    void *pixels)
{
    GL_CALL(
    glReadPixels(x, y, width, height,
                 GLCAST(inputComp),
                 GLCAST(outputDataType),
                 pixels);
    );
}

void GL::ReadPixels(const AARecti &readRect, GL::ColorComp inputComp,
                    GL::DataType outputDataType, void *pixels)
{
    GL::ReadPixels(readRect.GetMin().x, readRect.GetMin().y,
                   readRect.GetWidth(), readRect.GetHeight(),
                   inputComp, outputDataType, pixels);
}

void GL::Finish() { glFinish(); }
void GL::Flush() { glFlush(); }

void GL::Uniform(const String &name, int value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, float value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, bool value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Matrix3f &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Matrix4f &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Color &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Vector2 &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Vector3 &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(const String &name, const Vector4 &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<int> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<bool> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<float> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Color> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Vector2> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Vector3> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Vector4> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Matrix3> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}
void GL::Uniform(const String &name, const Array<Matrix4> &value)
{
    GL::Uniform( GL::GetUniformLocation(name), value);
}

void GL::Uniform(int location, int value)
{
    if (location >= 0) { GL_CALL( glUniform1i(location, value) ); }
}
void GL::Uniform(int location, float value)
{
    if (location >= 0) { GL_CALL( glUniform1f(location, value) ); }
}
void GL::Uniform(int location, bool value)
{
    if (location >= 0) { GL_CALL( glUniform1i(location, value ? 1 : 0) ); }
}
void GL::Uniform(int location, const Matrix3f& value)
{
    if (location >= 0) { GL_CALL( glUniformMatrix3fv(location, 1, false,
                                                     value.Data()) ); }
}
void GL::Uniform(int location, const Matrix4f& value)
{
    if (location >= 0) { GL_CALL( glUniformMatrix4fv(location, 1, false,
                                                     value.Data()) ); }
}
void GL::Uniform(int location, const Color &value)
{
    GL::Uniform(location, value.ToVector4());
}
void GL::Uniform(int location, const Vector2 &value)
{
    if (location >= 0) { GL_CALL( glUniform2fv(location, 1, value.Data()) ); }
}
void GL::Uniform(int location, const Vector3 &value)
{
    if (location >= 0) { GL_CALL( glUniform3fv(location, 1, value.Data()) ); }
}
void GL::Uniform(int location, const Vector4 &value)
{
    if (location >= 0) { GL_CALL( glUniform4fv(location, 1, value.Data()) ); }
}
void GL::Uniform(int location, const Array<int> &value)
{
    if (location >= 0) { GL_CALL( glUniform1iv(location, value.Size(),
                                               value.Data()) ); }
}
void GL::Uniform(int location, const Array<bool> &value)
{
    Array<int> valueInts;
    for (bool b : value) { valueInts.PushBack(b ? 1 : 0); }
    if (location >= 0) { GL_CALL( glUniform1iv(location, value.Size(),
                                               valueInts.Data()) ); }
}
void GL::Uniform(int location, const Array<float> &value)
{
    if (location >= 0) { GL_CALL( glUniform1fv(location, value.Size(),
                                               value.Data()) ); }
}
void GL::Uniform(int location, const Array<Color> &value)
{
    if (location >= 0) { GL_CALL( glUniform4fv(location, value.Size(),
                                   RCAST<const GLfloat*>(value.Data())) ); }
}
void GL::Uniform(int location, const Array<Vector2> &value)
{
    if (location >= 0) { GL_CALL( glUniform2fv(location, value.Size(),
                                   RCAST<const GLfloat*>(value.Data()) ) ); }
}
void GL::Uniform(int location, const Array<Vector3> &value)
{
    if (location >= 0) { GL_CALL( glUniform3fv(location, value.Size(),
                                   RCAST<const GLfloat*>(value.Data()) ) ); }
}
void GL::Uniform(int location, const Array<Vector4> &value)
{
    if (location >= 0) { GL_CALL( glUniform4fv(location, value.Size(),
                                   RCAST<const GLfloat*>(value.Data()) ) ); }
}
void GL::Uniform(int location, const Array<Matrix3> &value)
{
    if (location >= 0) { GL_CALL( glUniformMatrix3fv(location, value.Size(), false,
                                   RCAST<const GLfloat*>(value.Data()) ) ); }
}
void GL::Uniform(int location, const Array<Matrix4> &value)
{
    if (location >= 0) { GL_CALL( glUniformMatrix4fv(location, value.Size(), false,
                                   RCAST<const GLfloat*>(value.Data()) ) ); }
}

void GL::PixelStore(GL::Enum pixelStoreEnum, int n)
{
    GL_CALL( glPixelStorei(pixelStoreEnum, n) );
}

void GL::GenerateMipMap(GL::TextureTarget textureTarget)
{
    GL_CALL( glGenerateMipmap( GLCAST(textureTarget) ) );
}

void GL::TexImage2D(GL::TextureTarget textureTarget,
                    int textureWidth,
                    int textureHeight,
                    GL::ColorFormat textureColorFormat,
                    GL::ColorComp inputDataColorComp,
                    GL::DataType inputDataType,
                    const void *data)
{
    GL_CALL(
    glTexImage2D(GLCAST(textureTarget),
                 0,
                 GLCAST(textureColorFormat),
                 textureWidth,
                 textureHeight,
                 0,
                 GLCAST(inputDataColorComp),
                 GLCAST(inputDataType),
                 data)
                );
}

void GL::TexParameteri(GL::TextureTarget textureTarget,
                       GL::TexParameter textureParameter,
                       GLint value)
{
    GL_CALL(
    glTexParameteri(GLCAST(textureTarget),
                    GLCAST(textureParameter),
                    value); );
}

void GL::TexParameterf(GL::TextureTarget textureTarget,
                       GL::TexParameter textureParameter,
                       GLfloat value)
{
    GL_CALL(
    glTexParameterf(GLCAST(textureTarget),
                    GLCAST(textureParameter),
                    value); );
}

void GL::TexParameterFilter(GL::TextureTarget textureTarget,
                            GL::FilterMagMin filterMagMin,
                            GL::FilterMode filterMode)
{
    GL_CALL(
    GL::TexParameteri(textureTarget,
                      SCAST<GL::TexParameter>(filterMagMin),
                      GLCAST(filterMode)) );
}

void GL::TexParameterWrap(GL::TextureTarget textureTarget,
                          GL::WrapCoord wrapCoord,
                          GL::WrapMode wrapMode)
{
    GL_CALL(
    GL::TexParameteri(textureTarget,
                      SCAST<GL::TexParameter>(wrapCoord),
                      GLCAST(wrapMode)) );
}

void GL::GetTexImage(GL::TextureTarget textureTarget,
                     GL::ColorComp colorComp,
                     Byte *pixels)
{
    GL::GetTexImage(textureTarget, colorComp, GL::DataType::UnsignedByte, pixels);
}

void GL::GetTexImage(GL::TextureTarget textureTarget,
                     GL::ColorComp colorComp,
                     float *pixels)
{
    GL::GetTexImage(textureTarget, colorComp, GL::DataType::Float, pixels);
}

void GL::GetTexImage(GL::TextureTarget textureTarget,
                     GL::ColorComp colorComp,
                     GL::DataType dataType,
                     void *pixels)
{
    GL_CALL(
    glGetTexImage(GLCAST(textureTarget),
                  0,
                  GLCAST(colorComp),
                  GLCAST(dataType),
                  Cast<void*>(pixels)) );
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
    GL_CALL( glGetBooleanv(GLCAST(glEnum), &result) );
    *values = result;
}

int GL::GetInteger(GL::Enum glEnum)
{
    GLint result;
    GL::GetInteger(glEnum, &result);
    return result;
}
void GL::GetInteger(GL::Enum glEnum, int *values)
{
    GL_CALL( glGetIntegerv(glEnum, values) );
}

void GL::ActiveTexture(int activeTexture)
{
    ASSERT(activeTexture >= GL_TEXTURE0);
    GL_CALL( glActiveTexture(activeTexture) );
}

void GL::LineWidth(float lineWidth)
{
    if (GL::GetLineWidth() != lineWidth)
    {
        GL::GetActive()->m_lineWidth = lineWidth;
        GL_CALL( glLineWidth(lineWidth) );
    }
}

void GL::PointSize(float pointSize)
{
    GL_CALL( glPointSize(pointSize) );
}

void GL::GenFramebuffers(int n, GLId *glIds)
{
    GL_CALL( glGenFramebuffers(n, glIds) );
}

void GL::GenRenderBuffers(int n, GLId *glIds)
{
    GL_CALL( glGenRenderbuffers(n, glIds) );
}

void GL::GenTextures(int n, GLId *glIds)
{
    GL_CALL( glGenTextures(n, glIds) );
}

void GL::GenVertexArrays(int n, GLId *glIds)
{
    GL_CALL( glGenVertexArrays(n, glIds) );
}

void GL::GenBuffers(int n, GLId *glIds)
{
    GL_CALL( glGenBuffers(n, glIds) );
}

void GL::DeleteFramebuffers(int n, const GLId *glIds)
{
    GL_CALL( glDeleteFramebuffers(n, glIds) );
}

void GL::DeleteRenderBuffers(int n, const GLId *glIds)
{
    GL_CALL( glDeleteRenderbuffers(n, glIds) );
}

void GL::DeleteTextures(int n, const GLId *glIds)
{
    GL_CALL( glDeleteTextures(n, glIds) );
}

void GL::DeleteVertexArrays(int n, const GLId *glIds)
{
    GL_CALL( glDeleteVertexArrays(n, glIds) );
}

void GL::DeleteBuffers(int n, const GLId *glIds)
{
    GL_CALL( glDeleteBuffers(n, glIds) );
}

void GL::SetViewport(const AARect &viewportNDC)
{
    Vector2 minPx = GL::FromViewportPointNDCToViewportPoint(viewportNDC.GetMin());
    Vector2 maxPx = GL::FromViewportAmountNDCToViewportAmount(viewportNDC.GetMax());
    GL::SetViewport( AARecti(minPx.x, minPx.y, maxPx.x, maxPx.y) );
}

void GL::SetViewport(const AARecti &viewport)
{
    GL::SetViewport(viewport.GetMin().x, viewport.GetMin().y,
                    viewport.GetWidth(), viewport.GetHeight());
}

void GL::SetViewport(int x, int y, int width, int height)
{
    AARecti vpRect( Vector2i(x, y), Vector2i(x+width, y+height));
    if (GL::GetViewportRect() != vpRect)
    {
        GL *gl = GL::GetActive();
        if (gl) { gl->m_viewportRect = vpRect; }
        GL_CALL( glViewport(x, y, width, height) );

        if (gl && GLUniforms::GetActive())
        {
            GLUniforms::OnViewportChanged( GL::GetViewportRect() );
        }
    }
}

AARecti GL::GetViewportRect()
{
    GL *gl = GL::GetActive();
    return gl ? gl->m_viewportRect : AARecti::Zero;
}

Vector2i GL::GetViewportSize()
{
    return GL::GetViewportRect().GetSize();
}

float GL::GetViewportAspectRatio()
{
    Vector2i vpSize = GL::GetViewportSize();
    return SCAST<float>(vpSize.x) / Math::Max(vpSize.y, 1);
}

Vector2 GL::GetViewportPixelSize()
{
    return 1.0f / Vector2(GL::GetViewportSize());
}

const Array<GL::Attachment> &GL::GetDrawBuffers()
{
    return GL::GetActive()->m_drawBuffers;
}

GL::Attachment &GL::GetReadBuffer()
{
    return GL::GetActive()->m_readBuffer;
}

GL::BlendFactor GL::GetBlendSrcFactorColor()
{
    return GL::GetActive()->m_blendSrcFactorColor;
}

GL::BlendFactor GL::GetBlendDstFactorColor()
{
    return GL::GetActive()->m_blendDstFactorColor;
}

GL::BlendFactor GL::GetBlendSrcFactorAlpha()
{
    return GL::GetActive()->m_blendSrcFactorAlpha;
}

GL::BlendFactor GL::GetBlendDstFactorAlpha()
{
    return GL::GetActive()->m_blendDstFactorAlpha;
}

GL::BlendEquationE GL::GetBlendEquationColor()
{
    return GL::GetActive()->m_blendEquationColor;
}

GL::BlendEquationE GL::GetBlendEquationAlpha()
{
    return GL::GetActive()->m_blendEquationAlpha;
}

const AARecti& GL::GetScissorRect()
{
    if (!GL::IsEnabled(GL::Test::Scissor) ||
        GL::GetActive()->m_scissorRectPx == AARecti(-1,-1,-1,-1))
    {
        GL::GetActive()->m_scissorRectPx = GL::GetViewportRect();
    }
    return GL::GetActive()->m_scissorRectPx;
}

void GL::BufferData(GL::BindTarget target, int dataSize,
                    const void *data, GL::UsageHint usageHint)
{
    GL_CALL( glBufferData(GLCAST(target), dataSize, data, GLCAST(usageHint)) );
}


void GL::Render(const VAO *vao, GL::Primitive renderMode,
                int elementsCount, int startElementIndex)
{
    if (vao->IsIndexed())
    {
        GL::DrawElements(vao, renderMode, elementsCount, startElementIndex);
    }
    else
    {
        GL::DrawArrays(vao, renderMode, elementsCount, startElementIndex);
    }
}

void GL::DrawArrays(const VAO *vao, GL::Primitive primitivesMode,
                    int elementsCount, int startIndex)
{
    vao->Bind();
    GL_CALL( glDrawArrays( GLCAST(primitivesMode), startIndex, elementsCount) );
    vao->UnBind();
}

void GL::DrawElements(const VAO *vao, GL::Primitive primitivesMode,
                      int elementsCount, int startElementIndex)
{
    vao->Bind();
    GL_CALL( glDrawElements( GLCAST(primitivesMode),
                             elementsCount,
                             GLCAST(GL::DataType::UnsignedInt),
                             RCAST<const void*>(startElementIndex)) );
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
        case GL::BindTarget::Texture2D:
            if (gl) { gl->m_boundTextureId = glId; }
            glBindTexture( GLCAST(GL::BindTarget::Texture2D), glId);
        break;
        case GL::BindTarget::ShaderProgram:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundShaderProgramId = glId; }
            GL_CALL( glUseProgram(glId) );
        break;
        case GL::BindTarget::Framebuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl)
            {
                gl->m_boundDrawFramebufferId = gl->m_boundReadFramebufferId = glId;
            }
            GL_CALL( glBindFramebuffer( GLCAST(GL::BindTarget::Framebuffer),
                                        glId) );
        break;
        case GL::BindTarget::DrawFramebuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundDrawFramebufferId = glId; }
            GL_CALL( glBindFramebuffer( GLCAST(GL::BindTarget::DrawFramebuffer),
                                        glId) );
        break;
        case GL::BindTarget::ReadFramebuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundReadFramebufferId = glId; }
            GL_CALL( glBindFramebuffer( GLCAST(GL::BindTarget::ReadFramebuffer),
                                        glId) );
        break;
        case GL::BindTarget::VAO:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if(gl) { gl->m_boundVAOId = glId; }
            GL_CALL( glBindVertexArray(glId) );
        break;
        case GL::BindTarget::ElementArrayBuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundVBOElementsBufferId = glId; }
            GL_CALL( glBindBuffer( GLCAST(GL::BindTarget::ElementArrayBuffer), glId) );
        break;
        case GL::BindTarget::ArrayBuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundVBOArrayBufferId = glId; }
            GL_CALL( glBindBuffer( GLCAST(GL::BindTarget::ArrayBuffer), glId) );
        break;
        case GL::BindTarget::UniformBuffer:
            if (GL::IsBound(bindTarget, glId)) { return; }
            if (gl) { gl->m_boundUniformBufferId = glId; }
            GL_CALL( glBindBuffer( GLCAST(GL::BindTarget::UniformBuffer),
                                   glId) );
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
    return bindable ? GL::IsBound(bindable->GetGLBindTarget(), bindable->GetGLId()) :
                      false;
}

void GL::SetColorMask(bool maskR, bool maskG, bool maskB, bool maskA)
{
    GL *gl = GL::GetActive();
    std::array<bool,4> newColorMask = {{maskR, maskG, maskB, maskA}};
    if (!gl || (GL::GetColorMask() != newColorMask))
    {
        gl->m_colorMask = newColorMask;
        GL_CALL( glColorMask(maskR, maskG, maskB, maskA) );
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
    GL_CALL( glStencilOp(GLCAST(fail), GLCAST(zFail), GLCAST(zPass)) );
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
        GL_CALL( glStencilFunc(GLCAST(stencilFunction), stencilValue, mask) );
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
        GL_CALL( glDepthMask(writeDepth) );
    }
}

void GL::SetDepthFunc(GL::Function depthFunc)
{
    if (GL::GetDepthFunc() != depthFunc)
    {
        GL::GetActive()->m_depthFunc = depthFunc;
        GL_CALL( glDepthFunc( GLCAST(depthFunc) ) );
    }
}

void GL::SetCullFace(GL::Face cullFace)
{
    if (GL::GetCullFace() != cullFace)
    {
        GL::GetActive()->m_cullFace = cullFace;
        GL_CALL( glCullFace( GLCAST(cullFace) ) );
    }
}

void GL::SetWireframe(bool wireframe)
{
    if (GL::IsWireframe() != wireframe)
    {
        GL::PolygonMode(GL::Face::Back,         wireframe ? GL::Line : GL::Fill);
        GL::PolygonMode(GL::Face::Front,        wireframe ? GL::Line : GL::Fill);
        GL::PolygonMode(GL::Face::FrontAndBack, wireframe ? GL::Line : GL::Fill);
    }
}

Vector2 GL::FromPixelsPointToPixelPerfect(const Vector2 &winPoint)
{
    return Vector2::Floor(winPoint);
}

Vector2 GL::FromPointToPointNDC(const Vector2 &point, const Vector2 &rectSize)
{
    return (point / rectSize) * 2.0f - 1.0f;
}

Vector2 GL::FromPointNDCToPoint(const Vector2 &pointNDC, const Vector2 &rectSize)
{
    return (pointNDC * 0.5f + 0.5f) * rectSize;
}

Vector2 GL::FromAmountToAmountNDC(const Vector2 &amount,
                                  const Vector2 &rectSize)
{
    return ( (amount / rectSize) * 2.0f);
}

Vector2 GL::FromAmountNDCToAmount(const Vector2 &amountNDC,
                                  const Vector2 &rectSize)
{
    return (amountNDC * rectSize * 0.5f);
}

Vector2 GL::FromWindowPointToViewportPoint(const Vector2 &winPoint,
                                           const AARecti &viewport)
{
    return Vector2(winPoint - Vector2(viewport.GetMin()));
}

Vector2 GL::FromViewportPointToWindowPoint(const Vector2 &vpPoint)
{
    Vector2 winPoint = vpPoint + Vector2( GL::GetViewportRect().GetMin() );
    return winPoint;
}
Vector2 GL::FromViewportPointToWindowPoint(const Vector2i &vpPoint)
{
    return GL::FromViewportPointToWindowPoint( Vector2(vpPoint) );
}

Vector2 GL::FromWindowPointToViewportPoint(const Vector2 &winPoint)
{
    return GL::FromWindowPointToViewportPoint(winPoint, GL::GetViewportRect());
}
Vector2 GL::FromWindowPointToViewportPoint(const Vector2i &winPoint)
{
    return GL::FromWindowPointToViewportPoint( Vector2(winPoint) );
}

RectPoints GL::FromViewportRectToViewportRectNDCPoints(const Rect &vpRect)
{
    RectPoints points = vpRect.GetPoints();
    return {{GL::FromViewportPointToViewportPointNDC(points[0]),
             GL::FromViewportPointToViewportPointNDC(points[1]),
             GL::FromViewportPointToViewportPointNDC(points[2]),
             GL::FromViewportPointToViewportPointNDC(points[3])}};
}

Rect GL::FromViewportRectNDCToViewportRect(const Rect &vpRectNDC)
{
    return Rect( FromViewportPointNDCToViewportPoint(vpRectNDC.GetCenter()),
                 vpRectNDC.GetAxis(0),
                 FromViewportAmountNDCToViewportAmount(vpRectNDC.GetHalfSize()));
}

Rect GL::FromWindowRectToWindowRectNDC(const Rect &winRect)
{
    return Rect( FromWindowPointToWindowPointNDC(winRect.GetCenter()),
                 winRect.GetAxis(0),
                 FromWindowAmountToWindowAmountNDC(winRect.GetHalfSize()));
}

Rect GL::FromWindowRectNDCToWindowRect(const Rect &winRectNDC)
{
    return Rect( FromWindowPointNDCToWindowPoint(winRectNDC.GetCenter()),
                 winRectNDC.GetAxis(0),
                 FromWindowAmountNDCToWindowAmount(winRectNDC.GetHalfSize()));
}

AARect GL::FromViewportRectToViewportRectNDC(const AARect &vpRect)
{
    Vector2 min = GL::FromViewportPointToViewportPointNDC(vpRect.GetMin());
    Vector2 max = GL::FromViewportPointToViewportPointNDC(vpRect.GetMax());
    return AARect(min, max);
}

AARect GL::FromViewportRectNDCToViewportRect(const AARect &vpRectNDC)
{
    Vector2 min = GL::FromViewportPointNDCToViewportPoint(vpRectNDC.GetMin());
    Vector2 max = GL::FromViewportPointNDCToViewportPoint(vpRectNDC.GetMax());
    return AARect(min, max);
}

AARect GL::FromWindowRectToWindowRectNDC(const AARect &winRect)
{
    Vector2 min = GL::FromWindowPointToWindowPointNDC(winRect.GetMin());
    Vector2 max = GL::FromWindowPointToWindowPointNDC(winRect.GetMax());
    return AARect(min, max);
}

AARect GL::FromWindowRectNDCToWindowRect(const AARect &winRectNDC)
{
    Vector2 min = GL::FromWindowPointNDCToWindowPoint(winRectNDC.GetMin());
    Vector2 max = GL::FromWindowPointNDCToWindowPoint(winRectNDC.GetMax());
    return AARect(min, max);
}

Vector2 GL::FromWindowAmountToWindowAmountNDC(const Vector2 &winAmount)
{
    return (winAmount / Vector2(Window::GetActive()->GetSize())) * 2.0f;
}

Vector2 GL::FromWindowAmountNDCToWindowAmount(const Vector2 &winAmountNDC)
{
    return ((winAmountNDC * Vector2(Window::GetActive()->GetSize())) * 0.5f);
}

Vector2 GL::FromViewportAmountToViewportAmountNDC(const Vector2 &vpAmount)
{
    Vector2 vpSize ( Vector2i::Max(GL::GetViewportSize(), Vector2i::One) );
    return (vpAmount / vpSize) * 2.0f;
}

Vector2 GL::FromViewportAmountNDCToViewportAmount(const Vector2 &vpAmountNDC)
{
    return ((vpAmountNDC * Vector2(GL::GetViewportSize())) * 0.5f);
}

Vector2 GL::FromWindowPointNDCToWindowPoint(const Vector2 &winPointNDC)
{
    Vector2i winSize = Window::GetActive()->GetSize();
    return GL::FromPointNDCToPoint(winPointNDC, Vector2(winSize));
}

Vector2 GL::FromWindowPointToWindowPointNDC(const Vector2 &winPoint)
{
    Vector2i winSize( Window::GetActive()->GetSize() );
    return GL::FromPointToPointNDC(winPoint, Vector2(winSize));
}
Vector2 GL::FromWindowPointToWindowPointNDC(const Vector2i &winPoint)
{
    return GL::FromWindowPointToWindowPointNDC( Vector2(winPoint) );
}

Vector2 GL::FromViewportPointToViewportPointNDC(const Vector2 &vpPoint)
{
    Vector2i vpSize = Vector2i::Max( Vector2i::One, GL::GetViewportSize() );
    Vector2 res = GL::FromPointToPointNDC(vpPoint, Vector2(vpSize));
    return res;
}

Vector2 GL::FromViewportPointToViewportPointNDC(const Vector2i &vpPoint)
{
    return GL::FromViewportPointToViewportPointNDC( Vector2(vpPoint) );
}

Vector2 GL::FromViewportPointNDCToViewportPoint(const Vector2 &vpPointNDC)
{
    Vector2 vpSize( GL::GetViewportSize() );
    return GL::FromPointNDCToPoint(vpPointNDC, vpSize);
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
    return gl ? gl->m_cullFace : GL::Face::Back;
}

GLId GL::GetBoundId(GL::BindTarget bindTarget)
{
    switch(bindTarget)
    {
        case GL::BindTarget::Texture2D:
            return GL::GetActive()->m_boundTextureId;
        case GL::BindTarget::Framebuffer:
            return ( GL::GetBoundId(GL::BindTarget::DrawFramebuffer) ==
                     GL::GetBoundId(GL::BindTarget::ReadFramebuffer) ) ?
                       GL::GetBoundId(GL::BindTarget::DrawFramebuffer) : 0;
        case GL::BindTarget::DrawFramebuffer:
            return GL::GetActive()->m_boundDrawFramebufferId;
        case GL::BindTarget::ReadFramebuffer:
            return GL::GetActive()->m_boundReadFramebufferId;
        case GL::BindTarget::VAO:
            return GL::GetActive()->m_boundVAOId;
        case GL::BindTarget::ArrayBuffer:
            return GL::GetActive()->m_boundVBOArrayBufferId;
        case GL::BindTarget::ElementArrayBuffer:
            return GL::GetActive()->m_boundVBOElementsBufferId;
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
        case GL::ColorFormat::RGBA_UByte8:      return 1;
        case GL::ColorFormat::RGBA_Float16:     return 8;
        case GL::ColorFormat::Depth:            return 4;
        case GL::ColorFormat::Depth16:          return 2;
        case GL::ColorFormat::Depth24:          return 3;
        case GL::ColorFormat::Depth32:          return 4;
        case GL::ColorFormat::Depth32F:         return 16;
        case GL::ColorFormat::Depth24_Stencil8: return 4;
        case GL::ColorFormat::RGB10_A2_UByte:   return 4;
        case GL::ColorFormat::RGBA_Float32:     return 16;
    }
    return 0;
}

uint GL::GetPixelBytesSize(GL::ColorComp colorComp, GL::DataType dataType)
{
    return GL::GetNumComponents(colorComp) * GL::GetBytesSize(dataType);
}

uint GL::GetBytesSize(GL::DataType dataType)
{
    switch (dataType)
    {
        case GL::DataType::Byte:
        case GL::DataType::UnsignedByte:
            return sizeof(Byte);

        case GL::DataType::Short:
        case GL::DataType::UnsignedShort:
            return sizeof(short);

        case GL::DataType::Int:
        case GL::DataType::UnsignedInt:
            return sizeof(int);

        case GL::DataType::Float:
            return sizeof(float);

        case GL::DataType::Double:
            return sizeof(double);

        default: return -1;
    }
    ASSERT(false);
    return 0;
}

uint GL::GetNumComponents(GL::ColorComp colorComp)
{
    switch (colorComp)
    {
        case GL::ColorComp::RED:
        case GL::ColorComp::Depth:
        case GL::ColorComp::DepthStencil:
            return 1;

        case GL::ColorComp::RGB: return 3;
        case GL::ColorComp::RGBA: return 4;

        default: break;
    }
    ASSERT(false);
    return 0;
}

uint GL::GetNumComponents(GL::ColorFormat colorFormat)
{
    return GL::GetNumComponents( GL::GetColorCompFrom(colorFormat) );
}

GL::DataType GL::GetDataTypeFrom(GL::ColorFormat format)
{
    switch (format)
    {
        case GL::ColorFormat::RGBA_UByte8:
            return GL::DataType::UnsignedByte;

        case GL::ColorFormat::RGBA_Float16:
        case GL::ColorFormat::RGBA_Float32:
        case GL::ColorFormat::Depth24_Stencil8:
        case GL::ColorFormat::Depth:
        case GL::ColorFormat::Depth16:
        case GL::ColorFormat::Depth24:
        case GL::ColorFormat::Depth32:
        case GL::ColorFormat::Depth32F:
            return GL::DataType::Float;

        default: break;
    }
    ASSERT(false);
    return GL::DataType::Float;
}

GL::ColorComp GL::GetColorCompFrom(GL::ColorFormat format)
{
    switch (format)
    {
        case GL::ColorFormat::RGBA_UByte8:
        case GL::ColorFormat::RGBA_Float16:
        case GL::ColorFormat::RGBA_Float32:
        case GL::ColorFormat::RGB10_A2_UByte:
            return GL::ColorComp::RGBA;

        case GL::ColorFormat::Depth:
        case GL::ColorFormat::Depth16:
        case GL::ColorFormat::Depth24:
        case GL::ColorFormat::Depth32:
        case GL::ColorFormat::Depth32F:
        case GL::ColorFormat::Depth24_Stencil8:
            return GL::ColorComp::Depth;

        default: break;
    }
    ASSERT(false);
    return GL::ColorComp::RGB;
}

bool GL::IsDepthFormat(GL::ColorFormat format)
{
    return (format == GL::ColorFormat::Depth    ||
            format == GL::ColorFormat::Depth16  ||
            format == GL::ColorFormat::Depth24  ||
            format == GL::ColorFormat::Depth32  ||
            format == GL::ColorFormat::Depth32F ||
            format == GL::ColorFormat::Depth24_Stencil8);
}

void GL::BindUniformBufferToShader(const String &uniformBlockName,
                                   const ShaderProgram *sp,
                                   const IUniformBuffer *buffer)
{
    GL_CALL(
    GLuint blockIndex = glGetUniformBlockIndex(sp->GetGLId(),
                                               uniformBlockName.ToCString()) );
    GL_CALL( glUniformBlockBinding(sp->GetGLId(), blockIndex,
                                   buffer->GetBindingPoint()) );
}

void GL::PrintGLContext()
{
    Debug_Log("GL Context: ================");
    Debug_Peek(GL::GetViewportRect());
    Debug_Peek(GL::GetScissorRect());
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::VAO)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::ArrayBuffer)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::Framebuffer)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::DrawFramebuffer)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::ReadFramebuffer)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::ShaderProgram)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::Texture2D)) );
    Debug_Peek( SCAST<int>(GL::GetBoundId(GL::BindTarget::UniformBuffer)) );
    Debug_Peek(GL::GetColorMask());
    Debug_Peek(GL::GetLineWidth());
    Debug_Peek(GL::IsEnabled(GL::Test::Alpha));
    Debug_Peek(GL::IsEnabled(GL::Test::Blend));
    Debug_Peek(GL::IsEnabled(GL::Test::CullFace));
    Debug_Peek(GL::IsEnabled(GL::Test::Depth));
    Debug_Peek(GL::IsEnabled(GL::Test::DepthClamp));
    Debug_Peek(GL::IsEnabled(GL::Test::Scissor));
    Debug_Peek(GL::IsEnabled(GL::Test::Stencil));
    Debug_Peek(GL::GetDrawBuffers());
    Debug_Peek(GL::GetReadBuffer());
    Debug_Peek(GL::GetDepthMask());
    Debug_Peek(GL::GetDepthFunc());
    Debug_Peek(GL::GetClearColor());
    Debug_Peek(GL::GetCullFace());
    Debug_Peek(GL::GetPolygonMode(GL::Face::Back));
    Debug_Peek(GL::GetPolygonMode(GL::Face::Front));
    Debug_Peek(GL::GetPolygonMode(GL::Face::FrontAndBack));
    Debug_Peek(GL::GetStencilOp());
    Debug_Peek(GL::GetStencilFunc());
    Debug_Peek( SCAST<int>(GL::GetStencilValue()) );
    Debug_Peek( SCAST<int>(GL::GetStencilMask()) );
    Debug_Peek(GL::GetBlendDstFactorAlpha());
    Debug_Peek(GL::GetBlendDstFactorColor());
    Debug_Peek(GL::GetBlendEquationAlpha());
    Debug_Peek(GL::GetBlendEquationColor());
    Debug_Peek(GL::GetBlendSrcFactorAlpha());
    Debug_Peek(GL::GetBlendSrcFactorColor());

    Debug::PrintAllUniforms();

    Debug_Log("==========================");
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

void GL::SetActive(GL *gl)
{
    GL::s_activeGL = gl;
}

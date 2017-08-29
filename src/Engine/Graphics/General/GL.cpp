#include "Bang/GL.h"

#include "Bang/G_VAO.h"
#include "Bang/Debug.h"
#include "Bang/Screen.h"
#include "Bang/Matrix3.h"
#include "Bang/GLObject.h"
#include "Bang/G_Texture.h"
#include "Bang/G_ShaderProgram.h"
#include "Bang/GraphicPipeline.h"

void GL::ClearError()
{
    glGetError();
}

bool GL::CheckError(int line, const String &func, const String &file)
{
    GLenum glError;
    bool error = false;

    glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        const char *err = reinterpret_cast<const char*>(gluErrorString(glError));
        Debug_Error("OpenGL error: " << err << " At function " << func << " in "
                    << file << ":" << line);
        error = true;
    }
    return error;
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

void GL::ClearColorBuffer(const Color &clearColor,
                          bool clearR, bool clearG, bool clearB, bool clearA)
{
    bool colorMaskRBefore = GL::IsColorMaskR();
    bool colorMaskGBefore = GL::IsColorMaskG();
    bool colorMaskBBefore = GL::IsColorMaskB();
    bool colorMaskABefore = GL::IsColorMaskA();
    GL::SetColorMask(clearR, clearG, clearB, clearA);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    GL::SetColorMask(colorMaskRBefore, colorMaskGBefore,
                     colorMaskBBefore, colorMaskABefore);
}

void GL::ClearDepthBuffer(float clearDepth)
{
    glClearDepth(clearDepth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void GL::ClearStencilBuffer()
{
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
}

void GL::Enable(GLenum glEnum)
{
    glEnable(glEnum);
}

void GL::Disable(GLenum glEnum)
{
    glDisable(glEnum);
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
    GL::ClearError();

    glGetTexImage(GLCAST(textureTarget),
                  0,
                  GLCAST(GL::ColorComp::RGBA),
                  GLCAST(GL::DataType::UnsignedByte),
                  SCAST<void*>(pixels));

    GL_CheckError();
}

void GL::GetInteger(GL::Enum glEnum, int *values)
{
    glGetIntegerv(glEnum, values);
}

int GL::GetInteger(GL::Enum glEnum)
{
    GLint result;
    GL::GetInteger(glEnum, &result);
    return result;
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

void GL::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void GL::SetLineWidth(float lineWidth)
{
    glLineWidth(lineWidth);
}

void GL::Render(const G_VAO *vao, GL::Primitives primitivesMode,
                int elementsCount, int startIndex)
{
    vao->Bind();
    glDrawArrays( static_cast<GLint>(primitivesMode),
                  startIndex, elementsCount);
    vao->UnBind();
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

void GL::ApplyToShaderProgram(G_ShaderProgram *sp)
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

        float ar = 1.0f / Screen::GetAspectRatio();
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
    GL *gl = GL::GetActive();
    gl->m_colorMaskR = maskR;
    gl->m_colorMaskG = maskG;
    gl->m_colorMaskB = maskB;
    gl->m_colorMaskA = maskA;
    glColorMask(maskR, maskG, maskB, maskA);
}

void GL::SetViewProjMode(GL::ViewProjMode mode)
{
    GL *gl = GL::GetActive();
    gl->m_viewProjMode = mode;
}

void GL::SetStencilOp(GLenum zPassOp)
{
    GL *gl = GL::GetActive();
    if (gl->m_stencilOp != zPassOp)
    {
        gl->m_stencilOp = zPassOp;
        glStencilOp(GL_KEEP, GL_KEEP, zPassOp);
    }
}

void GL::SetStencilValue(Byte value)
{
    GL *gl = GL::GetActive();
    if (gl->m_stencilValue != value)
    {
        gl->m_stencilValue = value;
        glStencilFunc(IsStencilTest() ? GL_EQUAL : GL_ALWAYS,
                      gl->m_stencilValue, 0xFF);
    }
}

void GL::SetStencilWrite(bool writeStencil)
{
    if (writeStencil)
    {
        if (GetStencilOp() == GL_KEEP) { SetStencilOp(GL_REPLACE); }
        else { SetStencilOp(GetStencilOp()); }
    }
    else { SetStencilOp(GL_KEEP); }
}

void GL::SetStencilTest(bool testStencil)
{
    GL *gl = GL::GetActive();
    if (gl->m_testStencil != testStencil)
    {
        gl->m_testStencil = testStencil;
        glStencilFunc(testStencil ? GL_EQUAL : GL_ALWAYS,
                      GetStencilValue(), 0xFF);
    }
}
void GL::SetDepthWrite(bool writeDepth)
{
    GL *gl = GL::GetActive();
    if (gl->m_writeDepth != writeDepth)
    {
        gl->m_writeDepth = writeDepth;
        glDepthMask(gl->m_writeDepth ? GL_TRUE : GL_FALSE);
    }
}

void GL::SetDepthTest(bool testDepth)
{
    GL *gl = GL::GetActive();
    if (gl->m_testDepth != testDepth)
    {
        gl->m_testDepth = testDepth;
        glDepthFunc(gl->m_testDepth ? GL_LEQUAL : GL_ALWAYS);
    }
}

void GL::SetWireframe(bool wireframe)
{
    GL *gl = GL::GetActive();
    if (gl->m_wireframe != wireframe)
    {
        gl->m_wireframe = wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, gl->m_wireframe ? GL_LINE : GL_FILL);
    }
}

void GL::SetCullMode(GL::Face cullMode)
{
    GL *gl = GL::GetActive();
    if (cullMode != gl->m_cullMode)
    {
        gl->m_cullMode = cullMode;
        if (gl->m_cullMode != GL::Face::None)
        {
            GL::Enable(GL_CULL_FACE);
            glCullFace(GLint(gl->m_cullMode));
        }
        else { GL::Disable(GL_CULL_FACE); }
    }
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

GLenum GL::GetStencilOp() { return GL::GetActive()->m_stencilOp; }
Byte GL::GetStencilValue() { return GL::GetActive()->m_stencilValue; }

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

bool GL::IsStencilWrite()
{
    return GL::GetActive()->GetStencilOp() != GL_KEEP;
}
bool GL::IsStencilTest() { return GL::GetActive()->m_testStencil; }
bool GL::IsDepthWrite() { return GL::GetActive()->m_writeDepth; }
bool GL::IsDepthTest() { return GL::GetActive()->m_testDepth; }
bool GL::IsWireframe() { return GL::GetActive()->m_wireframe; }
GL::Face GL::GetCullMode() { return GL::GetActive()->m_cullMode; }

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
    GL::ColorComp colorOrder = GL::GetColorCompFrom(texFormat);
    GL::DataType dataType    = GL::GetDataTypeFrom(texFormat);
    return GL::GetPixelBytesSize(colorOrder, dataType);
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
    if (colorComp == GL::ColorComp::RGBA) { return 4; }
    if (colorComp == GL::ColorComp::RGB)  { return 3; }
    return 3;
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
    return GL::ColorComp::RGB;
}

GL *GL::GetActive()
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    return gp ? gp->GetGL() : nullptr;
}

GL::GL() {}

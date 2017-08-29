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
    GL::_Bind(bindTarget, glId);
    GL *gl = GL::GetActive();
    if (gl) { gl->OnBind(bindTarget, glId); }
}

void GL::_Bind(GL::BindTarget bindTarget, GLId glId)
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
    GL *gl = GL::GetActive();
    if (gl) { gl->OnUnBind(bindTarget); }
    else { gl->_Bind(bindTarget, 0); }
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
bool GL::IsColorMaskR() { return GL::GetActive()->m_colorMaskR; }
bool GL::IsColorMaskG() { return GL::GetActive()->m_colorMaskG; }
bool GL::IsColorMaskB() { return GL::GetActive()->m_colorMaskB; }
bool GL::IsColorMaskA() { return GL::GetActive()->m_colorMaskA; }
bool GL::IsStencilWrite()
{
    return GL::GetActive()->GetStencilOp() != GL_KEEP;
}
bool GL::IsStencilTest() { return GL::GetActive()->m_testStencil; }
bool GL::IsDepthWrite() { return GL::GetActive()->m_writeDepth; }
bool GL::IsDepthTest() { return GL::GetActive()->m_testDepth; }
bool GL::IsWireframe() { return GL::GetActive()->m_wireframe; }
GL::Face GL::GetCullMode() { return GL::GetActive()->m_cullMode; }
bool GL::IsBound(GL::BindTarget bindTarget, GLId glId)
{
    GL *gl = GL::GetActive();
    auto it = gl->m_glBoundIds.Find(bindTarget);
    if (it != gl->m_glBoundIds.End())
    {
        const std::stack<GLId>& boundIds = it->second;
        return !boundIds.empty() && boundIds.top() == glId;
    }
    return false;
}
const Matrix4&
GL::GetModelMatrix() { return GL::GetActive()->m_modelMatrix; }
const Matrix4&
GL::GetViewMatrix() { return GL::GetActive()->m_viewMatrix; }
const Matrix4&
GL::GetProjectionMatrix() { return GL::GetActive()->m_projectionMatrix; }
GLId GL::GetBoundId(GL::BindTarget bindTarget)
{
    GL *gl = GL::GetActive();
    if (!gl->m_glBoundIds.ContainsKey(bindTarget)) { return 0; }
    const std::stack<GLId> &boundIds = gl->m_glBoundIds.Get(bindTarget);
    return boundIds.empty() ? 0 : boundIds.top();
}

void GL::OnBind(GL::BindTarget bindTarget, GLId glId)
{
    if (!m_glBoundIds.ContainsKey(bindTarget))
    {
        m_glBoundIds.Add(bindTarget, std::stack<GLId>());
    }
    m_glBoundIds[bindTarget].push(glId);
}

void GL::OnUnBind(GL::BindTarget bindTarget)
{
    if (!m_glBoundIds.ContainsKey(bindTarget)) { return; }

    std::stack<GLId> &boundIds = m_glBoundIds.Get(bindTarget);
    if (!boundIds.empty())
    {
        const GLId currentId = GL::GetBoundId(bindTarget);
        boundIds.pop();
        const GLId previousBoundId = GetBoundId(bindTarget);
        if (currentId != previousBoundId)
        {
            GL::_Bind(bindTarget, previousBoundId);
        }
    }
}

uint GL::GetPixelBytesSize(GL::ColorInternalFormat texFormat)
{
    GL::ColorOrder colorOrder = GL::GetColorOrderFrom(texFormat);
    GL::DataType glDataType   = GL::GetDataTypeFrom(texFormat);

    uint numComps = 1;
    if (colorOrder == GL::ColorOrder::RGBA) { numComps = 4; }
    else if (colorOrder == GL::ColorOrder::RGB) { numComps = 3; }

    uint dataSize = 1;
    if (glDataType == GL::DataType::Float) { dataSize = 4; }

    return numComps * dataSize;
}

GL::DataType GL::GetDataTypeFrom(GL::ColorInternalFormat texFormat)
{
    if (texFormat == GL::ColorInternalFormat::RGBA_UByte8)
    {
        return GL::DataType::UnsignedByte;
    }
    else if (texFormat == GL::ColorInternalFormat::RGBA_Float16 ||
             texFormat == GL::ColorInternalFormat::RGBA_Float32)
    {
        return GL::DataType::Float;
    }
    return GL::DataType::Float;
}

GL::ColorOrder GL::GetColorOrderFrom(GL::ColorInternalFormat texFormat)
{
    if (texFormat == GL::ColorInternalFormat::RGBA_UByte8   ||
        texFormat == GL::ColorInternalFormat::RGBA_Float16 ||
        texFormat == GL::ColorInternalFormat::RGBA_Float32)
    {
        return GL::ColorOrder::RGBA;
    }
    return GL::ColorOrder::RGB;
}

GL *GL::GetActive()
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    return gp ? gp->GetGL() : nullptr;
}

GL::GL() {}

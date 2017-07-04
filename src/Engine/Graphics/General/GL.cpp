#include "Bang/GL.h"

#include "Bang/Debug.h"
#include "Bang/GLObject.h"
#include "Bang/G_Texture.h"
#include "Bang/GLContext.h"
#include "Bang/G_ShaderProgram.h"
#include "Bang/G_GraphicPipeline.h"

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

bool GL::CheckG_FramebufferError()
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
    glColorMask(clearR, clearG, clearB, clearA);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glColorMask(true, true, true, true);
}

void GL::ClearDepthBuffer(float clearDepth)
{
    glClearDepth(clearDepth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void GL::ClearStencilBuffer()
{
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

void GL::SetViewProjMode(GL::ViewProjMode mode)
{
    GL::GetGLContext()->SetViewProjMode(mode);
}

void GL::SetWriteDepth(bool writeDepth)
{
    GL::GetGLContext()->SetWriteDepth(writeDepth);
}

void GL::SetTestDepth(bool testDepth)
{
    GL::GetGLContext()->SetTestDepth(testDepth);
}

void GL::SetWireframe(bool wireframe)
{
    GL::GetGLContext()->SetWireframe(wireframe);
}

void GL::SetCullMode(const GL::CullMode cullMode)
{
    GL::GetGLContext()->SetCullMode(cullMode);
}

void GL::SetModelMatrix(const Matrix4 &model)
{
    GL::GetGLContext()->SetModelMatrix(model);
}

void GL::SetViewMatrix(const Matrix4 &view)
{
    GL::GetGLContext()->SetViewMatrix(view);
}

void GL::SetProjectionMatrix(const Matrix4 &projection)
{
    GL::GetGLContext()->SetProjectionMatrix(projection);
}

void GL::SetZNearFar(float zNear, float zFar)
{
    GL::GetGLContext()->SetZNearFar(zNear, zFar);
}

void GL::ApplyContextToShaderProgram(G_ShaderProgram *sp)
{
    GL::GetGLContext()->ApplyToShaderProgram(sp);
}

void GL::Render(const G_VAO *vao, GL::RenderMode renderMode, int elementsCount,
                int startIdx)
{
    return GL::GetGLContext()->Render(vao, renderMode, elementsCount, startIdx);
}

bool GL::IsWriteDepth()
{
    return GL::GetGLContext()->IsWriteDepth();
}

bool GL::IsTestDepth()
{
    return GL::GetGLContext()->IsTestDepth();
}

bool GL::IsWireframe()
{
    return GL::GetGLContext()->IsWireframe();
}

GL::CullMode GL::GetCullMode()
{
    return GL::GetGLContext()->GetCullMode();
}

const Matrix4 &GL::GetModelMatrix()
{
    return GL::GetGLContext()->GetModelMatrix();
}

const Matrix4 &GL::GetViewMatrix()
{
    return GL::GetGLContext()->GetViewMatrix();
}

const Matrix4 &GL::GetProjectionMatrix()
{
    return GL::GetGLContext()->GetProjectionMatrix();
}

void GL::Bind(const GLObject *bindable)
{
    GL::Bind(bindable->GetGLBindTarget(), bindable->GetGLId());
}

void GL::Bind(GL::BindTarget bindTarget, GLId glId)
{
    GLContext *glContext = GL::GetGLContext();
    if (glContext) { glContext->OnBind(bindTarget, glId); }
    GL::_Bind(bindTarget, glId);
}

void GL::_Bind(GL::BindTarget bindTarget, GLId glId)
{
    switch (bindTarget)
    {
        case BindTarget::Texture2D:
            glBindTexture(GL_TEXTURE_2D, glId);
        break;
        case BindTarget::ShaderProgram:
            glUseProgram(glId);
        break;
        case BindTarget::G_Framebuffer:
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
    GLContext *glc = GL::GetGLContext();
    if (glc)
    {
        glc->OnUnBind(bindTarget);
    }
    else
    {
        GL::_Bind(bindTarget, 0);
    }
}

bool GL::IsBound(const GLObject *bindable)
{
    return GL::IsBound(bindable->GetGLBindTarget(), bindable->GetGLId());
}

bool GL::IsBound(GL::BindTarget bindTarget, GLId glId)
{
    GLContext *glc = GL::GetGLContext();
    if (glc) { return glc->IsBound(bindTarget, glId); }
    return false;
}

GL::GL()
{
}

GLContext *GL::GetGLContext()
{
    G_GraphicPipeline *gp = G_GraphicPipeline::GetActive();
    return gp ? gp->GetGLContext() : nullptr;
}

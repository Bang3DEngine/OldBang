#include "GL.h"

#include "GLContext.h"
#include "GraphicPipeline.h"

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

void GL::ApplyToShaderProgram(ShaderProgram *sp)
{
    GL::GetGLContext()->ApplyToShaderProgram(sp);
}

void GL::Render(const VAO *vao, GL::RenderMode renderMode, int elementsCount,
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

void GL::SaveToImage(const Texture *tex, const String &filepath)
{
    // TODO
}

GL::GL()
{
}

GLContext *GL::GetGLContext()
{
    return GraphicPipeline::GetActive()->GetGLContext();
}


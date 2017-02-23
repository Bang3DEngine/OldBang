#include "GL.h"

#include "GLContext.h"
#include "GraphicPipeline.h"

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

void GL::Apply()
{
    GL::GetGLContext()->Apply();
}

void GL::Reset()
{
    GL::GetGLContext()->Reset();
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

GL::GL()
{
}

GLContext *GL::GetGLContext()
{
    return GraphicPipeline::GetActive()->GetGLContext();
}


#include "Bang/GLObject.h"

#include "Bang/GL.h"

USING_NAMESPACE_BANG

GLObject::GLObject() {}

GLId GLObject::GetGLId() const
{
    return m_idGL;
}

GL_BindTarget GLObject::GetGLBindTarget() const
{
    return GL_BindTarget::None;
}

void GLObject::Bind() const
{
    GL::Bind(this);
}
void GLObject::UnBind() const
{
    // GL::UnBind(this);
}

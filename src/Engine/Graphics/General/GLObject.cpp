#include "Bang/GLObject.h"

GLObject::GLObject() {}

GLId GLObject::GetGLId() const
{
    return m_idGL;
}

GL::BindTarget GLObject::GetGLBindTarget() const
{
    return GL::BindTarget::None;
}

void GLObject::Bind() const
{
    GL::Bind(this);
}
void GLObject::UnBind() const
{
    GL::UnBind(this);
}

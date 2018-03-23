#include "Bang/IBO.h"

#include "Bang/GL.h"

USING_NAMESPACE_BANG

IBO::IBO()
{
    GL::GenBuffers(1, &m_idGL);
}

IBO::~IBO()
{
    GL::DeleteBuffers(1, &m_idGL);
}

void IBO::Fill(const void *data, int dataSize, GL::UsageHint usage)
{
    Bind();
    GL::BufferData(GetGLBindTarget(), dataSize, data, usage);
    UnBind();
}

GL::BindTarget IBO::GetGLBindTarget() const
{
    return GL::BindTarget::ElementArrayBuffer;
}

void IBO::Bind() const
{
    GL::Bind(this);
}
void IBO::UnBind() const
{
    GL::UnBind(this);
}

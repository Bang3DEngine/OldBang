#include "Bang/G_Texture.h"

G_Texture::G_Texture()
{
    glGenTextures(1, &m_idGL);
    SetFilterMode(m_filterMode);
    SetWrapMode(m_wrapMode);
    SetInternalFormat(m_internalFormat);
}

G_Texture::G_Texture(GL::TextureTarget texTarget) : G_Texture()
{
    m_target = texTarget;
}

G_Texture::G_Texture(const G_Texture &t) : GLObject(t)
{
    m_width = t.m_width;
    m_height = t.m_height;
    m_filterMode = t.m_filterMode;
    m_wrapMode = t.m_wrapMode;
    m_textureUnit = t.m_textureUnit;
    m_internalFormat = t.m_internalFormat;
    m_target = t.m_target;
    m_data = t.m_data;
}

G_Texture::~G_Texture()
{
    if (m_data) { delete[] m_data; }
    glDeleteTextures(1, &m_idGL);
}

void G_Texture::SetInternalFormat(GL::ColorInternalFormat internalFormat)
{
    m_internalFormat = internalFormat;
}

void G_Texture::SetTarget(GL::TextureTarget target)
{
    m_target = target;
}

void G_Texture::SetFilterMode(GL::FilterMode filterMode)
{
    m_filterMode = filterMode;
    Bind();
    glTexParameteri(GLCAST(m_target),
                    GL_TEXTURE_MAG_FILTER,
                    GLCAST(filterMode));
    glTexParameteri(GLCAST(m_target),
                    GL_TEXTURE_MIN_FILTER,
                    GLCAST(filterMode));
    UnBind();
}

void G_Texture::SetWrapMode(GL::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    Bind();
    glTexParameteri(GLCAST(m_target),
                    GL_TEXTURE_WRAP_S,
                    GLint(wrapMode));
    glTexParameteri(GLCAST(m_target),
                    GL_TEXTURE_WRAP_T,
                    GLCAST(wrapMode));
    glTexParameteri(GLCAST(m_target),
                    GL_TEXTURE_WRAP_R,
                    GLCAST(wrapMode));
    UnBind();
}

void G_Texture::SetTextureUnit(int textureSlot)
{
    m_textureUnit = textureSlot;
}

int G_Texture::GetWidth() const
{
    return m_width;
}

int G_Texture::GetHeight() const
{
    return m_height;
}

GL::DataType G_Texture::GetInternalDataType() const
{
    return GL::GetDataTypeFrom( GetInternalFormat() );
}

GL::ColorInternalFormat G_Texture::GetInternalFormat() const
{
    return m_internalFormat;
}

uint G_Texture::GetBytesSize() const
{
    return m_width * m_height * GL::GetPixelBytesSize(m_internalFormat);
}

GL::FilterMode G_Texture::GetFilterMode() const
{
    return m_filterMode;
}

GL::WrapMode G_Texture::GetWrapMode() const
{
    return m_wrapMode;
}

int G_Texture::GetTextureUnit() const
{
    return m_textureUnit;
}

void G_Texture::BindToTextureUnit(int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    Bind();
}

GL::BindTarget G_Texture::GetGLBindTarget() const
{
    return GL::BindTarget::Texture2D;
}

#include "Bang/G_Texture.h"

G_Texture::G_Texture()
{
    GL::GenTextures(1, &m_idGL);
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
    m_size = Vector2i(t.GetWidth(), t.GetHeight());
    m_filterMode = t.m_filterMode;
    m_wrapMode = t.m_wrapMode;
    m_textureUnit = t.m_textureUnit;
    m_internalFormat = t.m_internalFormat;
    m_target = t.m_target;
}

G_Texture::~G_Texture()
{
    GL::DeleteTextures(1, &m_idGL);
}

void G_Texture::SetInternalFormat(GL::ColorFormat internalFormat)
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
    GL::TexParameterFilter(m_target, GL::FilterMagMin::Mag, filterMode);
    GL::TexParameterFilter(m_target, GL::FilterMagMin::Min, filterMode);
    UnBind();
}

void G_Texture::SetWrapMode(GL::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    Bind();
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapS, wrapMode);
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapT, wrapMode);
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapR, wrapMode);
    UnBind();
}

void G_Texture::SetTextureUnit(int textureSlot)
{
    m_textureUnit = textureSlot;
}

int G_Texture::GetWidth() const
{
    return m_size.x;
}

int G_Texture::GetHeight() const
{
    return m_size.y;
}

const Vector2i &G_Texture::GetSize() const
{
    return m_size;
}

GL::DataType G_Texture::GetInternalDataType() const
{
    return GL::GetDataTypeFrom( GetInternalFormat() );
}

GL::ColorFormat G_Texture::GetInternalFormat() const
{
    return m_internalFormat;
}

uint G_Texture::GetBytesSize() const
{
    return GetWidth() * GetHeight() * GL::GetPixelBytesSize(m_internalFormat);
}

GL::TextureTarget G_Texture::GetTextureTarget() const
{
    return m_target;
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
    GL::ActiveTexture(GL_TEXTURE0 + textureUnit);
    Bind();
}

void G_Texture::SetWidth(int width) { m_size.x = width; }
void G_Texture::SetHeight(int height) { m_size.y = height; }

GL::BindTarget G_Texture::GetGLBindTarget() const
{
    return GL::BindTarget::Texture2D;
}

#include "Bang/Texture.h"

Texture::Texture()
{
    GL::GenTextures(1, &m_idGL);
    SetFilterMode(m_filterMode);
    SetWrapMode(m_wrapMode);
    SetInternalFormat(m_internalFormat);
}

Texture::~Texture()
{
    GL::DeleteTextures(1, &m_idGL);
}

Texture::Texture(GL::TextureTarget texTarget) : Texture()
{
    m_target = texTarget;
}

Texture::Texture(const Texture &t) : GLObject(t)
{
    m_size = Vector2i(t.GetWidth(), t.GetHeight());
    m_filterMode = t.m_filterMode;
    m_wrapMode = t.m_wrapMode;
    m_textureUnit = t.m_textureUnit;
    m_internalFormat = t.m_internalFormat;
    m_target = t.m_target;
}

void Texture::SetInternalFormat(GL::ColorFormat internalFormat)
{
    m_internalFormat = internalFormat;
}

void Texture::SetTarget(GL::TextureTarget target)
{
    m_target = target;
}

void Texture::SetFilterMode(GL::FilterMode filterMode)
{
    m_filterMode = filterMode;
    Bind();
    GL::TexParameterFilter(m_target, GL::FilterMagMin::Mag, filterMode);
    GL::TexParameterFilter(m_target, GL::FilterMagMin::Min, filterMode);
    UnBind();
}

void Texture::SetWrapMode(GL::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    Bind();
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapS, wrapMode);
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapT, wrapMode);
    GL::TexParameterWrap(m_target, GL::WrapCoord::WrapR, wrapMode);
    UnBind();
}

void Texture::SetTextureUnit(int textureSlot)
{
    m_textureUnit = textureSlot;
}

int Texture::GetWidth() const
{
    return m_size.x;
}

int Texture::GetHeight() const
{
    return m_size.y;
}

const Vector2i &Texture::GetSize() const
{
    return m_size;
}

GL::DataType Texture::GetInternalDataType() const
{
    return GL::GetDataTypeFrom( GetInternalFormat() );
}

GL::ColorFormat Texture::GetInternalFormat() const
{
    return m_internalFormat;
}

uint Texture::GetBytesSize() const
{
    return GetWidth() * GetHeight() * GL::GetPixelBytesSize(m_internalFormat);
}

GL::TextureTarget Texture::GetTextureTarget() const
{
    return m_target;
}

GL::FilterMode Texture::GetFilterMode() const
{
    return m_filterMode;
}

GL::WrapMode Texture::GetWrapMode() const
{
    return m_wrapMode;
}

int Texture::GetTextureUnit() const
{
    return m_textureUnit;
}

void Texture::BindToTextureUnit(int textureUnit) const
{
    GL::ActiveTexture(GL_TEXTURE0 + textureUnit);
    Bind();
}

void Texture::SetWidth(int width) { m_size.x = width; }
void Texture::SetHeight(int height) { m_size.y = height; }

GL::BindTarget Texture::GetGLBindTarget() const
{
    return GL::BindTarget::Texture2D;
}

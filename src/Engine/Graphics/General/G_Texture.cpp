#include "Bang/G_Texture.h"

G_Texture::G_Texture()
{
    glGenTextures(1, &m_idGL);
    SetFilterMode(m_filterMode);
    SetWrapMode(m_wrapMode);
    SetFormat(m_format);
}

G_Texture::G_Texture(Target texTarget) : G_Texture()
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
    m_format = t.m_format;
    m_target = t.m_target;
    m_data = t.m_data;
}

G_Texture::~G_Texture()
{
    if (m_data) { delete[] m_data; }
    glDeleteTextures(1, &m_idGL);
}

void G_Texture::SetTarget(G_Texture::Target target)
{
    m_target = target;
}

void G_Texture::SetFormat(G_Texture::Format format)
{
    m_format = format;
}

void G_Texture::SetFilterMode(G_Texture::FilterMode filterMode)
{
    m_filterMode = filterMode;
    Bind();
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GLint(filterMode));
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GLint(filterMode));
    UnBind();
}

void G_Texture::SetWrapMode(G_Texture::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    Bind();
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GLint(wrapMode));
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GLint(wrapMode));
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GLint(wrapMode));
    UnBind();
}

void G_Texture::SetTextureUnit(int textureSlot)
{
    m_textureUnit = textureSlot;
}

G_Texture::Format G_Texture::GetFormat() const
{
    return m_format;
}

int G_Texture::GetWidth() const
{
    return m_width;
}

int G_Texture::GetHeight() const
{
    return m_height;
}

GLint G_Texture::GetGLDataType() const
{
    return G_Texture::GetGLDataTypeFrom(m_format);
}

GLint G_Texture::GetGLFormat() const
{
    return G_Texture::GetGLFormatFrom(m_format);
}

GLint G_Texture::GetGLInternalFormat() const
{
    return G_Texture::GetGLInternalFormatFrom(m_format);
}

uint G_Texture::GetBytesSize() const
{
    return m_width * m_height * G_Texture::GetPixelBytesSize(m_format);
}

uint G_Texture::GetPixelBytesSize(G_Texture::Format texFormat)
{
    GLint glNumComponents = G_Texture::GetGLFormatFrom(texFormat);
    GLint glDataType      = G_Texture::GetGLDataTypeFrom(texFormat);

    uint numComps = 1;
    if (glNumComponents == GL_RGBA) { numComps = 4; }

    uint dataSize = 1;
    if (glDataType == GL_FLOAT) { dataSize = 4; }

    return numComps * dataSize;
}

GLint G_Texture::GetGLDataTypeFrom(G_Texture::Format texFormat)
{
    if (texFormat == G_Texture::Format::RGBA_Byte8)
    {
        return GL_UNSIGNED_BYTE;
    }
    else if (texFormat == G_Texture::Format::RGBA_Float16 ||
             texFormat == G_Texture::Format::RGBA_Float32)
    {
        return GL_FLOAT;
    }
    return GL_FLOAT;
}

GLint G_Texture::GetGLFormatFrom(G_Texture::Format texFormat)
{
    if (texFormat == G_Texture::Format::RGBA_Byte8   ||
        texFormat == G_Texture::Format::RGBA_Float16 ||
        texFormat == G_Texture::Format::RGBA_Float32)
    {
        return GL_RGBA;
    }
    return GL_RGB;
}

GLint G_Texture::GetGLInternalFormatFrom(G_Texture::Format texFormat)
{
    if (texFormat == G_Texture::Format::RGBA_Byte8)
    {
        return GL_RGBA;
    }
    else if (texFormat == G_Texture::Format::RGBA_Float16)
    {
        return GL_RGBA16F;
    }
    else if (texFormat == G_Texture::Format::RGBA_Float32)
    {
        return GL_RGBA32F;
    }
    return GL_RGBA;
}

G_Texture::FilterMode G_Texture::GetFilterMode() const
{
    return m_filterMode;
}

G_Texture::WrapMode G_Texture::GetWrapMode() const
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

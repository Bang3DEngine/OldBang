#include "Texture.h"

Texture::Texture()
{
    glGenTextures(1, &m_idGL);
    SetFilterMode(m_filterMode);
    SetWrapMode(m_wrapMode);
    SetFormat(m_format);
}

Texture::Texture(Target texTarget) : Texture()
{
    m_target = texTarget;
}

Texture::~Texture()
{
    if (m_data) { delete[] m_data; }
    glDeleteTextures(1, &m_idGL);
}

void Texture::SetTarget(Texture::Target target)
{
    m_target = target;
}

void Texture::SetFormat(Texture::Format format)
{
    m_format = format;
}

void Texture::SetFilterMode(Texture::FilterMode filterMode)
{
    m_filterMode = filterMode;
    Bind();
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GLint(filterMode));
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GLint(filterMode));
    UnBind();
}

void Texture::SetWrapMode(Texture::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    Bind();
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GLint(wrapMode));
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GLint(wrapMode));
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GLint(wrapMode));
    UnBind();
}

void Texture::SetTextureUnit(int textureSlot)
{
    m_textureUnit = textureSlot;
}

Texture::Format Texture::GetFormat() const
{
    return m_format;
}

int Texture::GetWidth() const
{
    return m_width;
}

int Texture::GetHeight() const
{
    return m_height;
}

GLint Texture::GetGLDataType() const
{
    return Texture::GetGLDataTypeFrom(m_format);
}

GLint Texture::GetGLFormat() const
{
    return Texture::GetGLFormatFrom(m_format);
}

GLint Texture::GetGLInternalFormat() const
{
    return Texture::GetGLInternalFormatFrom(m_format);
}

unsigned int Texture::GetBytesSize() const
{
    return m_width * m_height * Texture::GetPixelBytesSize(m_format);
}

unsigned int Texture::GetPixelBytesSize(Texture::Format texFormat)
{
    GLint glNumComponents = Texture::GetGLFormatFrom(texFormat);
    GLint glDataType      = Texture::GetGLDataTypeFrom(texFormat);

    unsigned int numComps = 1;
    if (glNumComponents == GL_RGBA) { numComps = 4; }

    unsigned int dataSize = 1;
    if (glDataType == GL_FLOAT) { dataSize = 4; }

    return numComps * dataSize;
}

GLint Texture::GetGLDataTypeFrom(Texture::Format texFormat)
{
    if (texFormat == Texture::Format::RGBA_Byte8)
    {
        return GL_UNSIGNED_BYTE;
    }
    else if (texFormat == Texture::Format::RGBA_Float32)
    {
        return GL_FLOAT;
    }
}

GLint Texture::GetGLFormatFrom(Texture::Format texFormat)
{
    if (texFormat == Texture::Format::RGBA_Byte8 ||
        texFormat == Texture::Format::RGBA_Float32)
    {
        return GL_RGBA;
    }
}

GLint Texture::GetGLInternalFormatFrom(Texture::Format texFormat)
{
    if (texFormat == Texture::Format::RGBA_Byte8)
    {
        return GL_RGBA;
    }
    else if (texFormat == Texture::Format::RGBA_Float32)
    {
        return GL_RGBA32F;
    }
}

Texture::FilterMode Texture::GetFilterMode() const
{
    return m_filterMode;
}

Texture::WrapMode Texture::GetWrapMode() const
{
    return m_wrapMode;
}

int Texture::GetTextureUnit() const
{
    return m_textureUnit;
}

void Texture::BindToTextureUnit(int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    Bind();
}


void Texture::Bind() const
{
    // TODO: Change Prebind for all kind of texture targets.
    PreBind(GL_TEXTURE_BINDING_2D);
    glBindTexture(m_target, m_idGL);
}

void Texture::UnBind() const
{
    glBindTexture(m_target, PreUnBind(GL_TEXTURE_BINDING_2D));
}

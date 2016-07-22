#include "Texture.h"

Texture::Texture()
{
    glGenTextures(1, &m_idGL);
    SetFilterMode(m_filterMode);
    SetWrapMode(m_wrapMode);
    m_glTextureGetIntegerType = GL_TEXTURE_2D;
}

Texture::Texture(TextureType glTextureTarget) : Texture()
{
    m_glTextureType = glTextureTarget;
    if (m_glTextureType == Texture1D)
        m_glTextureGetIntegerType = GL_TEXTURE_BINDING_1D;
    else if (m_glTextureType == Texture2D)
        m_glTextureGetIntegerType = GL_TEXTURE_BINDING_2D;
    else if (m_glTextureType == Texture3D)
        m_glTextureGetIntegerType = GL_TEXTURE_BINDING_3D;
    else if (m_glTextureType == TextureCubeMap)
        m_glTextureGetIntegerType = GL_TEXTURE_BINDING_CUBE_MAP;

}

Texture::~Texture()
{
    if (m_data ) delete m_data;
    glDeleteTextures(1, &m_idGL);
}

void Texture::SetGLInternalType(GLint glInternalType)
{
    m_internalType = glInternalType;
}

void Texture::SetGLInternalFormat(GLint glInternalFormat)
{
    m_internalFormat = glInternalFormat;
}

void Texture::SetGLFormat(GLint glFormat)
{
    m_format = glFormat;
}

void Texture::SetFilterMode(Texture::FilterMode filterMode)
{
    this->m_filterMode = filterMode;
    Bind();
    glTexParameteri(m_glTextureType, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(m_glTextureType, GL_TEXTURE_MIN_FILTER, filterMode);
    UnBind();
}

void Texture::SetWrapMode(Texture::WrapMode wrapMode)
{
    this->m_wrapMode = wrapMode;
    Bind();
    glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_R, wrapMode);
    UnBind();
}

void Texture::SetTextureUnit(int textureSlot)
{
    this->m_textureUnit = textureSlot;
}

int Texture::GetWidth() const
{
    return m_width;
}

int Texture::GetHeight() const
{
    return m_height;
}

int Texture::GetNumComponents() const
{
    return m_numComponents;
}

GLint Texture::GetGLInternalType() const
{
    return m_internalType;
}

GLint Texture::GetGLInternalFormat() const
{
    return m_internalFormat;
}

GLint Texture::GetGLFormat() const
{
    return m_format;
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
    glBindTexture(GL_TEXTURE_2D, m_idGL);
}


void Texture::Bind() const
{
    PreBind(GL_TEXTURE_BINDING_2D);
    glBindTexture(GL_TEXTURE_2D, m_idGL);
}

void Texture::UnBind() const
{
    glBindTexture(m_glTextureType, PreUnBind(GL_TEXTURE_BINDING_2D));
}

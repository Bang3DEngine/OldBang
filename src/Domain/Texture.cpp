#include "Texture.h"

Texture::Texture(TextureType glTextureTarget) :
    width(0), height(0), numComponents(0),
    filterMode(FilterMode::Linear),
    wrapMode(WrapMode::Repeat),
    textureSlot(0),
    data(nullptr),
    glTextureType(glTextureTarget)
{
    glGenTextures(1, &idgl);
    SetFilterMode(filterMode);
    SetWrapMode(wrapMode);

    if(glTextureType == Texture1D)
        glTextureGetIntegerType = GL_TEXTURE_BINDING_1D;
    else if(glTextureType == Texture2D)
        glTextureGetIntegerType = GL_TEXTURE_BINDING_2D;
    else if(glTextureType == Texture3D)
        glTextureGetIntegerType = GL_TEXTURE_BINDING_3D;
    else if(glTextureType == TextureCubeMap)
        glTextureGetIntegerType = GL_TEXTURE_BINDING_CUBE_MAP;

}

Texture::~Texture()
{
    if(data != nullptr) delete data;
    glDeleteTextures(1, &idgl);
}

void Texture::SetFilterMode(Texture::FilterMode filterMode)
{
    this->filterMode = filterMode;
    Bind();
    glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, filterMode);
    UnBind();
}

void Texture::SetWrapMode(Texture::WrapMode wrapMode)
{
    this->wrapMode = wrapMode;
    Bind();
    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_R, wrapMode);
    UnBind();
}

void Texture::SetTextureSlot(int textureSlot)
{
    this->textureSlot = textureSlot;
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}

int Texture::GetNumComponents() const
{
    return numComponents;
}

Texture::FilterMode Texture::GetFilterMode() const
{
    return filterMode;
}

Texture::WrapMode Texture::GetWrapMode() const
{
    return wrapMode;
}

int Texture::GetTextureSlot() const
{
    return textureSlot;
}

void Texture::Bind() const
{
    PreBind(GL_ACTIVE_TEXTURE, 1);
    glActiveTexture(GL_TEXTURE0 + textureSlot);

    PreBind(GL_TEXTURE_BINDING_2D, 0);
    glBindTexture(GL_TEXTURE_2D, idgl);
}

void Texture::UnBind() const
{
    glBindTexture(glTextureType, PreUnBind(0));
    glActiveTexture( PreUnBind(1) );
}

#include "Texture.h"

Texture::Texture(GLint glTextureTarget) :
    width(0), height(0), numComponents(0),
    filterMode(FilterMode::Linear),
    wrapMode(WrapMode::Repeat),
    textureSlot(0),
    rgbs(nullptr),
    glTextureTarget(glTextureTarget)
{
    glGenTextures(1, &idgl);
    SetFilterMode(filterMode);
    SetWrapMode(wrapMode);

    if(glTextureTarget == GL_TEXTURE_1D)
        glTextureTargetGetInteger = GL_TEXTURE_BINDING_1D;
    else if(glTextureTarget == GL_TEXTURE_2D)
        glTextureTargetGetInteger = GL_TEXTURE_BINDING_2D;
    else if(glTextureTarget == GL_TEXTURE_3D)
        glTextureTargetGetInteger = GL_TEXTURE_BINDING_3D;
    else if(glTextureTarget == GL_TEXTURE_CUBE_MAP)
        glTextureTargetGetInteger = GL_TEXTURE_BINDING_CUBE_MAP;

}

Texture::~Texture()
{
    if(rgbs != nullptr) delete rgbs;
    glDeleteTextures(1, &idgl);
}

void Texture::SetFilterMode(Texture::FilterMode filterMode)
{
    this->filterMode = filterMode;
    Bind();
    glTexParameteri(glTextureTarget, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(glTextureTarget, GL_TEXTURE_MIN_FILTER, filterMode);
    UnBind();
}

void Texture::SetWrapMode(Texture::WrapMode wrapMode)
{
    this->wrapMode = wrapMode;
    Bind();
    glTexParameteri(glTextureTarget, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(glTextureTarget, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(glTextureTarget, GL_TEXTURE_WRAP_R, wrapMode);
    UnBind();
}

void Texture::SetTextureSlot(int textureSlot)
{
    this->textureSlot = textureSlot;
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
    glBindTexture(glTextureTarget, PreUnBind(0));
    glActiveTexture( PreUnBind(1) );
}

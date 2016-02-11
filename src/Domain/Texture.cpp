#include "Texture.h"

Texture::Texture(int textureSlot) : width(0), height(0), numComponents(0),
                                    filterMode(FilterMode::Linear),
                                    wrapMode(WrapMode::Repeat),
                                    rgbs(nullptr)
{
    glGenTextures(1, &idgl);
    this->textureSlot = textureSlot;

    SetFilterMode(filterMode);
    SetWrapMode(wrapMode);
}

Texture::Texture(int textureSlot, const std::string &filepath) : Texture(textureSlot)
{
    LoadFromFile(filepath);
}

Texture::~Texture()
{
    if(rgbs != nullptr) delete rgbs;
    glDeleteTextures(1, &idgl);
}

void Texture::LoadFromFile(const std::string &filepath)
{
    Bind();

    rgbs = FileLoader::LoadImage(filepath, &numComponents, &width, &height);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbs);
    glGenerateMipmap(GL_TEXTURE_2D);

    UnBind();
}

void Texture::CreateEmpty(int width, int height)
{
    if(rgbs != nullptr) delete rgbs;
    Bind();

    this->width = width;
    this->height = height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    UnBind();
}

void Texture::SetFilterMode(Texture::FilterMode filterMode)
{
    this->filterMode = filterMode;
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    UnBind();
}

void Texture::SetWrapMode(Texture::WrapMode wrapMode)
{
    this->wrapMode = wrapMode;
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    UnBind();
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
    glBindTexture(GL_TEXTURE_2D, PreUnBind(0));
    glActiveTexture( PreUnBind(1) );
}

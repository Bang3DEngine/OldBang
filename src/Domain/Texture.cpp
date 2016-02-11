#include "Texture.h"

Texture::Texture(int textureSlot) : width(0), height(0), numComponents(0), rgbs(nullptr)
{
    glGenTextures(1, &idgl);
    this->textureSlot = textureSlot;
}

Texture::~Texture()
{
    glDeleteTextures(1, &idgl);
}

void Texture::LoadFromFile(const std::string &filepath)
{
    Bind();

    rgbs = FileLoader::LoadImage(filepath, &numComponents, &width, &height);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbs);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    UnBind();
}

int Texture::GetTextureSlot() const
{
    return textureSlot;
}

void Texture::Bind() const
{
    PreBind(GL_ACTIVE_TEXTURE, 1);
    glActiveTexture(textureSlot);

    PreBind(GL_TEXTURE_BINDING_2D, 0);
    glBindTexture(GL_TEXTURE_2D, idgl);
}

void Texture::UnBind() const
{
    glActiveTexture( PreUnBind(1) );
    glBindBuffer(GL_TEXTURE_2D, PreUnBind(0));
}

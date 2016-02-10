#include "Texture.h"

Texture::Texture() : width(0), height(0), numComponents(0), rgbs(nullptr)
{
    glGenTextures(1, &idgl);
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

void Texture::Bind() const
{
    GLuint lastActive; glGetIntegerv(GL_ACTIVE_TEXTURE, &lastActive);
    lastActiveTextureSlots.push(lastActive);

    PreBind(GL_TEXTURE_BINDING_2D);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, idgl);
}

void Texture::UnBind() const
{
    glBindBuffer(GL_TEXTURE_2D, PreUnBind());

    glActiveTexture(lastActiveTextureSlots.top());
    lastActiveTextureSlots.pop();
}

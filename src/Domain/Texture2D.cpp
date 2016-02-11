#include "Texture2D.h"

Texture2D::Texture2D() : Texture(GL_TEXTURE_2D)
{
}

Texture2D::Texture2D(const std::string &filepath) : Texture2D()
{
    LoadFromFile(filepath);
}

Texture2D::~Texture2D()
{
}

void Texture2D::LoadFromFile(const std::string &filepath)
{
    Bind();

    rgbs = FileLoader::LoadImage(filepath, &numComponents, &width, &height);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbs);
    glGenerateMipmap(GL_TEXTURE_2D);

    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    if(rgbs != nullptr) delete rgbs;
    Bind();

    this->width = width;
    this->height = height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    UnBind();
}

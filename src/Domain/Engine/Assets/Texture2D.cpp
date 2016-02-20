#include "Texture2D.h"

Texture2D::Texture2D() : Texture(TextureType::Texture2D)
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
    this->filepath = filepath;
    unsigned char *loadedData = FileReader::ReadImage(filepath, &numComponents, &width, &height);
    Fill(loadedData, width, height, numComponents);
    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    Fill(nullptr, width, height, numComponents);
}

void Texture2D::Resize(int width, int height)
{
    Fill(data, width, height, numComponents);
}

void Texture2D::Fill(unsigned char *newData, int width, int height, int numComponents)
{
    if(this->data != nullptr && this->data != newData)
        delete this->data;

    this->data = newData;
    this->width = width;
    this->height = height;
    this->numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

void Texture2D::Write(std::ostream &f) const
{

}

void Texture2D::Read(std::istream &f)
{
    filepath = FileReader::ReadString(f);
    Logger_Log("Texture 2D read: " << filepath);
    LoadFromFile(filepath );
}

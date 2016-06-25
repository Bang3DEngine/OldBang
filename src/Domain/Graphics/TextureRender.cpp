#include "TextureRender.h"
#include "Logger.h"
TextureRender::TextureRender() : Texture(TextureType::Texture2D)
{
    SetFilterMode(Texture::FilterMode::Nearest);
    SetWrapMode(Texture::WrapMode::Clamp);
}

TextureRender::~TextureRender()
{

}

void TextureRender::CreateEmpty(int width, int height)
{
    Fill(nullptr, width, height, numComponents);
}

void TextureRender::Resize(int width, int height)
{
    Fill(data, width, height, numComponents);
}

void TextureRender::Fill(unsigned char *newData, int width, int height, int numComponents)
{
    if(this->data  && this->data != newData)
        delete this->data;

    this->data = newData;
    this->width = width;
    this->height = height;
    this->numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    UnBind();
}

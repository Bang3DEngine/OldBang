#include "TextureRender.h"

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
    if(data != nullptr) delete data;
    Bind();

    this->width = width;
    this->height = height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    UnBind();
}

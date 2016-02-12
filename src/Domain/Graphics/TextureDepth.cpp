#include "TextureDepth.h"

TextureDepth::TextureDepth() : Texture(TextureType::Texture2D)
{

}

TextureDepth::~TextureDepth()
{

}

void TextureDepth::Resize(int width, int height)
{
    CreateEmpty(width, height);
}

void TextureDepth::CreateEmpty(int width, int height)
{
    this->width = width;
    this->height = height;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    UnBind();
}

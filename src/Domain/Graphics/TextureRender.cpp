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
    Fill(nullptr, width, height, m_numComponents);
}

void TextureRender::Resize(int width, int height)
{
    Fill(p_data, width, height, m_numComponents);
}

void TextureRender::Fill(unsigned char *newData, int width, int height, int numComponents)
{
    if(this->p_data  && this->p_data != newData)
        delete this->p_data;

    this->p_data = newData;
    this->m_width = width;
    this->m_height = height;
    this->m_numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, p_data);
    UnBind();
}

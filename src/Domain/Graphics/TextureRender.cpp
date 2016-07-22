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
    Fill(m_data, width, height, m_numComponents);
}

void TextureRender::Fill(unsigned char *newData, int width, int height, int numComponents)
{
    if (this->m_data  && this->m_data != newData)
        delete this->m_data;

    this->m_data = newData;
    this->m_width = width;
    this->m_height = height;
    this->m_numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_BYTE, m_data);
    UnBind();
}

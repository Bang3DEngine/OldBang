#include "TextureRender.h"
#include "Debug.h"

TextureRender::TextureRender() : Texture2D()
{
    SetFilterMode(Texture::FilterMode::Nearest);
    SetWrapMode(Texture::WrapMode::Clamp);
    SetGLInternalFormat(GL_RGBA32F);
    SetGLFormat(GL_RGBA);
    SetGLType(GL_FLOAT);
}

TextureRender::~TextureRender()
{

}
/*
void TextureRender::CreateEmpty(int width, int height)
{
    Fill(nullptr, width, height);
}

void TextureRender::Resize(int width, int height)
{
    Fill(m_data, width, height);
}

void TextureRender::Fill(unsigned char *newData, int width, int height)
{
    if (this->m_data  && this->m_data != newData)
        delete this->m_data;

    this->m_data = newData;
    this->m_width = width;
    this->m_height = height;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormat, width, height, 0, m_glFormat, m_glType, m_data);
    UnBind();
}
*/

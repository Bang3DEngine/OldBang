#include "Texture2D.h"
#include "Persistence.h"

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

void Texture2D::LoadFromFile(const std::string &imageFilepath)
{
    Bind();
    this->m_filepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(imageFilepath,
                                                      &m_width, &m_height,
                                                      &m_numComponents);
    Fill(loadedData, m_width, m_height, m_numComponents);
    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    Fill(nullptr, width, height, m_numComponents);
}

void Texture2D::Resize(int width, int height)
{
    Fill(p_data, width, height, m_numComponents);
}

void Texture2D::Fill(unsigned char *newData,
                     int width, int height,
                     int numComponents)
{
    if(this->p_data  && this->p_data != newData)
        delete this->p_data;

    this->p_data = newData;
    this->m_width = width;
    this->m_height = height;
    this->m_numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, p_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

std::string Texture2D::GetImageRelativeFilepath() const
{
    return m_filepath;
}

void Texture2D::Write(std::ostream &f) const
{
    f << "<Texture2D>" << std::endl;
    FileWriter::WriteFilepath(this->m_filepath, f);
    std::string fm = GetFilterMode() == FilterMode::Nearest ?
                "Nearest" : "Linear";
    FileWriter::Write(fm, f);
    f << "</Texture2D>" << std::endl;
}

void Texture2D::Read(std::istream &f)
{
    m_filepath = FileReader::ReadString(f);
    LoadFromFile(m_filepath);
    std::string filterMode = FileReader::ReadString(f);
    if(filterMode == "Nearest")
    {
        SetFilterMode(FilterMode::Nearest);
    }
    else if(filterMode == "Linear")
    {
        SetFilterMode(FilterMode::Linear);
    }
}

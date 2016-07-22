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
    Fill(m_data, width, height, m_numComponents);
}

void Texture2D::Fill(unsigned char *newData,
                     int width, int height,
                     int numComponents)
{
    if (this->m_data  && this->m_data != newData)
        delete this->m_data;

    this->m_data = newData;
    this->m_width = width;
    this->m_height = height;
    this->m_numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_internalType, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

std::string Texture2D::GetImageRelativeFilepath() const
{
    return m_filepath;
}

std::string Texture2D::GetTag() const
{
    return "Texture2D";
}

void Texture2D::WriteInternal(std::ostream &f) const
{
    Asset::WriteInternal(f);

    FileWriter::WriteFilepath(this->m_filepath, f);

    FilterMode filterMode = GetFilterMode();
    std::string fmName = "";
    if (filterMode == FilterMode::Nearest)
    {
        fmName = "Nearest";
    }
    else if (filterMode == FilterMode::Linear)
    {
        fmName = "Linear";
    }
    FileWriter::Write(fmName, f);
}

void Texture2D::ReadInternal(std::istream &f)
{
    Asset::ReadInternal(f);

    m_filepath = FileReader::ReadString(f);
    LoadFromFile(m_filepath);
    std::string filterMode = FileReader::ReadString(f);
    if (filterMode == "Nearest")
    {
        SetFilterMode(FilterMode::Nearest);
    }
    else if (filterMode == "Linear")
    {
        SetFilterMode(FilterMode::Linear);
    }
}

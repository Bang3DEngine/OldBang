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
    this->imageFilepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(imageFilepath,
                                                      &width, &height,
                                                      &numComponents);
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

void Texture2D::Fill(unsigned char *newData,
                     int width, int height,
                     int numComponents)
{
    if(this->data  && this->data != newData)
        delete this->data;

    this->data = newData;
    this->width = width;
    this->height = height;
    this->numComponents = numComponents;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

std::string Texture2D::GetImageRelativeFilepath() const
{
    return imageFilepath;
}

#ifdef BANG_EDITOR
void Texture2D::Write(std::ostream &f) const
{
    f << "<Texture2D>" << std::endl;
    FileWriter::WriteFilepath(this->imageFilepath, f);
    std::string fm = GetFilterMode() == FilterMode::Nearest ?
                "Nearest" : "Linear";
    FileWriter::Write(fm, f);
    f << "</Texture2D>" << std::endl;
}

void Texture2D::Read(std::istream &f)
{
    imageFilepath = FileReader::ReadString(f);
    LoadFromFile(imageFilepath);
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
#endif

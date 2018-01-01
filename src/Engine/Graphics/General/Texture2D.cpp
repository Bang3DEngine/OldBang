#include "Bang/Texture2D.h"

#include "Bang/Resources.h"

USING_NAMESPACE_BANG

Texture2D::Texture2D() : Texture(GL::TextureTarget::Texture2D)
{
    CreateEmpty(1,1);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Import(const Image<Byte> &image)
{
    if (image.GetData())
    {
        SetWidth(image.GetWidth());
        SetHeight(image.GetHeight());

        SetInternalFormat(GL::ColorFormat::RGBA_UByte8);
        Fill(image.GetData(),
             GetWidth(), GetHeight(),
             GL::ColorComp::RGBA,
             GL::DataType::UnsignedByte);
    }
}

void Texture2D::CreateEmpty(int width, int height)
{
    GL::ColorComp colorComp =
            (GetInternalFormat() == GL::ColorFormat::Depth24_Stencil8) ?
                GL::ColorComp::Depth : GL::ColorComp::RGB;
    Fill(nullptr, width, height, colorComp, GL::DataType::UnsignedByte, false);
}

void Texture2D::Resize(int width, int height)
{
    if (width != GetWidth() || height != GetHeight())
    {
        CreateEmpty(width, height);
    }
}

void Texture2D::Fill(const Color &fillColor,
                     int width, int height,
                     bool genMipMaps)
{
    Array<Color> inputData = Array<Color>(width * height, fillColor);
    Fill( RCAST<const Byte*>(inputData.Data()), width, height,
          GL::ColorComp::RGBA, GL::DataType::Float, genMipMaps);
}

void Texture2D::Fill(const Byte *newData,
                     int width, int height,
                     GL::ColorComp inputDataColorComp,
                     GL::DataType inputDataType,
                     bool genMipMaps)
{
    SetWidth(width);
    SetHeight(height);

    GLId prevBoundId = GL::GetBoundId(GL::BindTarget::Texture2D);
    Bind();
    GL::TexImage2D(GetTextureTarget(),
                   GetWidth(), GetHeight(),
                   GetInternalFormat(),
                   inputDataColorComp,
                   inputDataType,
                   newData);
    if (genMipMaps && GetWidth() > 0 && GetHeight() > 0)
    {
        GenerateMipMaps();
    }
    GL::Bind(GL::BindTarget::Texture2D, prevBoundId);
}

void Texture2D::GenerateMipMaps() const
{
    ASSERT(GL::IsBound(this));
    GL::GenerateMipMap( GetTextureTarget() );
}

void Texture2D::SetAlphaCutoff(float alphaCutoff)
{
    m_alphaCutoff = alphaCutoff;
}

float Texture2D::GetAlphaCutoff() const
{
    return m_alphaCutoff;
}

Color Texture2D::GetColorFromArray(const float *pixels, int i)
{
    return Color(pixels[i+0], pixels[i+1], pixels[i+2], pixels[i+3]);
}

Color Texture2D::GetColorFromArray(const Byte *pixels, int i)
{
    return Color(pixels[i+0] / 255.0f, pixels[i+1] / 255.0f,
                 pixels[i+2] / 255.0f, pixels[i+3] / 255.0f);
}


void Texture2D::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);

    if (xmlInfo.Contains("FilterMode"))
    { SetFilterMode( xmlInfo.Get<GL::FilterMode>("FilterMode") ); }

    if (xmlInfo.Contains("WrapMode"))
    { SetWrapMode( xmlInfo.Get<GL::WrapMode>("WrapMode") ); }

    if (xmlInfo.Contains("AlphaCutoff"))
    { SetAlphaCutoff( xmlInfo.Get<float>("AlphaCutoff") ); }
}

void Texture2D::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    xmlInfo->Set("FilterMode", GetFilterMode());
    xmlInfo->Set("WrapMode", GetWrapMode());
    xmlInfo->Set("AlphaCutoff", GetAlphaCutoff());
}

void Texture2D::Import(const Path &imageFilepath)
{
    RH<ImageResourceb> img = Resources::Load<ImageResourceb>(imageFilepath);
    if (img) { Import( *(img.Get()) ); }

    Path importFilepath = ImportFilesManager::GetImportFilePath(imageFilepath);
    ImportXMLFromFile(importFilepath);
}

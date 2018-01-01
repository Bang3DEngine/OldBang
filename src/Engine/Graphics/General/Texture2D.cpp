#include "Bang/Texture2D.h"

#include "Bang/Resources.h"

USING_NAMESPACE_BANG

Texture2D::Texture2D() : Texture(GL_TextureTarget::Texture2D)
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

        SetInternalFormat(GL_ColorFormat::RGBA_UByte8);
        Fill(image.GetData(),
             GetWidth(), GetHeight(),
             GL_ColorComp::RGBA,
             GL_DataType::UnsignedByte);
    }
}

void Texture2D::CreateEmpty(int width, int height)
{
    GL_ColorComp colorComp =
            (GetInternalFormat() == GL_ColorFormat::Depth24_Stencil8) ?
                GL_ColorComp::Depth : GL_ColorComp::RGB;
    Fill(nullptr, width, height, colorComp, GL_DataType::UnsignedByte);
}

void Texture2D::Resize(int width, int height)
{
    if (width != GetWidth() || height != GetHeight())
    {
        CreateEmpty(width, height);
    }
}

void Texture2D::Fill(const Color &fillColor,
                     int width, int height)
{
    Array<Color> inputData = Array<Color>(width * height, fillColor);
    Fill( RCAST<const Byte*>(inputData.Data()), width, height,
          GL_ColorComp::RGBA, GL_DataType::Float);
}

void Texture2D::Fill(const Byte *newData,
                     int width, int height,
                     GL_ColorComp inputDataColorComp,
                     GL_DataType inputDataType)
{
    SetWidth(width);
    SetHeight(height);

    GLId prevBoundId = GL::GetBoundId(GL_BindTarget::Texture2D);
    Bind();
    GL::TexImage2D(GetTextureTarget(),
                   GetWidth(), GetHeight(),
                   GetInternalFormat(),
                   inputDataColorComp,
                   inputDataType,
                   newData);
    GL::Bind(GL_BindTarget::Texture2D, prevBoundId);
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
    { SetFilterMode( xmlInfo.Get<GL_FilterMode>("FilterMode") ); }

    if (xmlInfo.Contains("WrapMode"))
    { SetWrapMode( xmlInfo.Get<GL_WrapMode>("WrapMode") ); }

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

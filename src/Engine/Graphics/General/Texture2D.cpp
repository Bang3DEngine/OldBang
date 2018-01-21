#include "Bang/Texture2D.h"

#include "Bang/GL.h"
#include "Bang/Resources.h"

USING_NAMESPACE_BANG

Texture2D::Texture2D() : Texture(GL::TextureTarget::Texture2D)
{
    CreateEmpty(1,1);
}

Texture2D::~Texture2D()
{
}

void Texture2D::CreateEmpty(int width, int height)
{
    GL::ColorComp colorComp =
            (GetInternalFormat() == GL::ColorFormat::Depth24_Stencil8) ?
                GL::ColorComp::Depth : GL::ColorComp::RGB;
    Fill(nullptr, width, height, colorComp, GL::DataType::UnsignedByte);
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
          GL::ColorComp::RGBA, GL::DataType::Float);
}

void Texture2D::Fill(const Byte *newData,
                     int width, int height,
                     GL::ColorComp inputDataColorComp,
                     GL::DataType inputDataType)
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

    if (newData && GetWidth() > 0 && GetHeight() > 0)
    {
        GL::GenerateMipMap(GL::TextureTarget::Texture2D);
    }

    GL::Bind(GL::BindTarget::Texture2D, prevBoundId);

    PropagateTextureChanged();
}

void Texture2D::GenerateMipMaps() const
{
    ASSERT(GL::IsBound(this));
    GL::GenerateMipMap( GetTextureTarget() );
}

void Texture2D::SetAlphaCutoff(float alphaCutoff)
{
    if (alphaCutoff != GetAlphaCutoff())
    {
        m_alphaCutoff = alphaCutoff;
        PropagateTextureChanged();
    }
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

template <class T>
void GetTexImageInto_T(const Texture2D *tex, T *pixels)
{
    GLId prevBound = GL::GetBoundId(GL::BindTarget::Texture2D);
    tex->Bind();
    GL::GetTexImage(tex->GetTextureTarget(), pixels);
    GL::Bind(GL::BindTarget::Texture2D, prevBound);
}

void Texture2D::GetTexImageInto(Byte *pixels) const
{ GetTexImageInto_T<Byte>(this, pixels); }
void Texture2D::GetTexImageInto(float *pixels) const
{ GetTexImageInto_T<float>(this, pixels); }

int Texture2D::GetNumComponents() const
{
    return GL::GetNumComponents( GetInternalFormat() );
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

    Path importFilepath = ImportFilesManager::GetImportFilepath(imageFilepath);
    ImportXMLFromFile(importFilepath);
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


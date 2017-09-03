#include "Bang/G_Image.h"

#include "Bang/Debug.h"
#include "Bang/ImageIO.h"
#include "Bang/ImportFilesManager.h"

G_Image::G_Image()
{

}

G_Image::G_Image(int width, int height)
{
    Create(width, height);
}

void G_Image::Create(int width, int height)
{
    m_size = Vector2i(width, height);
    m_pixels.Resize(m_size.x * m_size.y * 4);
}

void G_Image::Create(int width, int height, const Color &backgroundColor)
{
    Create(width, height);
    for (int i = 0; i < GetHeight(); ++i)
    {
        for (int j = 0; j < GetWidth(); ++j)
        {
            SetPixel(j, i, backgroundColor);
        }
    }
}

void G_Image::SetPixel(int x, int y, const Color& color)
{
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = SCAST<Byte>(color.r * 255);
    m_pixels[coord + 1] = SCAST<Byte>(color.g * 255);
    m_pixels[coord + 2] = SCAST<Byte>(color.b * 255);
    m_pixels[coord + 3] = SCAST<Byte>(color.a * 255);
}

void G_Image::Copy(const G_Image &image,
                   const Recti &srcCopyRect,
                   const Recti &dstCopyRect,
                   ResizeMode resizeMode)
{

}

void G_Image::Resize(const Vector2i &newSize, ResizeMode resizeMode)
{
    Resize(newSize.x, newSize.y, resizeMode);
}
void G_Image::Resize(const int newWidth, int newHeight, ResizeMode resizeMode)
{
    if (newWidth == GetWidth() && newHeight == GetHeight()) { return; }

    G_Image original = *this;

    Vector2 sizeProp(original.GetWidth()  / SCAST<float>(newWidth),
                     original.GetHeight() / SCAST<float>(newHeight));

    Create(newWidth, newHeight);
    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < newWidth; ++x)
        {
            Color newColor;
            if (resizeMode == ResizeMode::Nearest)
            {
                Vector2 oriCoord = Vector2(x,y) * sizeProp;
                int nearestX = Math::Round(oriCoord.x);
                int nearestY = Math::Round(oriCoord.y);
                newColor = original.GetPixel(nearestX, nearestY);
            }
            else
            {
                Vector2 oriTopLeftF  = Vector2(  x,   y) * sizeProp;
                Vector2 oriBotRightF = Vector2(x+1, y+1) * sizeProp;
                Vector2i oriTopLeft(Math::Floor(oriTopLeftF.x),
                                    Math::Floor(oriTopLeftF.y));
                Vector2i oriBotRight(Math::Ceil(oriBotRightF.x),
                                     Math::Ceil(oriBotRightF.y));

                // Debug_Log( Vector2(x,y) << " maps to: [" << oriTopLeft << ", "
                //            << oriBotRight << "]");
                newColor = Color::Zero;
                for (int oriY = oriTopLeft.y; oriY < oriBotRight.y; ++oriY)
                {
                    for (int oriX = oriTopLeft.x; oriX < oriBotRight.x; ++oriX)
                    {
                        newColor += original.GetPixel(oriX, oriY);
                    }
                }

                int pixels = (oriBotRight.x-oriTopLeft.x) *
                             (oriBotRight.y-oriTopLeft.y);
                newColor /= Math::Max(pixels, 1);
            }
            SetPixel(x, y, newColor);
        }
    }
}

Byte *G_Image::GetData() { return &m_pixels[0]; }
const Byte *G_Image::GetData() const { return &m_pixels[0]; }

Color G_Image::GetPixel(int x, int y) const
{
    const int coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0] / 255.0f,
                 m_pixels[coord + 1] / 255.0f,
                 m_pixels[coord + 2] / 255.0f,
                 m_pixels[coord + 3] / 255.0f);
}

int G_Image::GetWidth() const { return m_size.x; }
int G_Image::GetHeight() const { return m_size.y; }
const Vector2i& G_Image::GetSize() const { return m_size; }

void G_Image::InvertVertically()
{
    G_Image img = *this;
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            Color c = img.GetPixel(x, y);
            SetPixel(x, GetHeight() - y - 1, c);
        }
    }
}

void G_Image::SaveToFile(const Path &filepath) const
{
    ImageIO::Export(filepath, *this);
}

G_Image G_Image::LoadFromData(int width, int height,
                              const Array<Byte> &rgbaByteData)
{
    G_Image img(width, height);
    img.m_pixels = rgbaByteData;
    ASSERT(rgbaByteData.Size() == (img.GetWidth() * img.GetHeight() * 4));
    return img;
}

void G_Image::Import(const Path &imageFilepath)
{
    bool ok;
    ImageIO::Import(imageFilepath, this, &ok);

    if (!ok)
    {
        Debug_Error("Error loading the image '" << imageFilepath <<
                     "', couldn't open/read the file.");
    }
}

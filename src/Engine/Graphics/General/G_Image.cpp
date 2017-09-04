#include "Bang/G_Image.h"

#include "Bang/Rect.h"
#include "Bang/Debug.h"
#include "Bang/ImageIO.h"
#include "Bang/ImportFilesManager.h"

template<class T>
G_ImageG<T>::G_ImageG()
{

}

template<class T>
G_ImageG<T>::G_ImageG(int width, int height)
{
    Create(width, height);
}

template<class T>
void G_ImageG<T>::Create(int width, int height)
{
    m_size = Vector2i(width, height);
    m_pixels.Resize(m_size.x * m_size.y * 4);
}

template<class T>
void G_ImageG<T>::Create(int width, int height, const Color &backgroundColor)
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

template<class T>
G_ImageG<T> G_ImageG<T>::GetSubImage(const Recti &subCoords) const
{
    Vector2i subSize = subCoords.GetSize();
    G_ImageG<T> subImage(subSize.x, subSize.y);
    for (int y = 0; y < subSize.y; ++y)
    {
        for (int x = 0; x < subSize.x; ++x)
        {
            subImage.SetPixel(x, y, GetPixel(x + subCoords.GetMin().x,
                                             y + subCoords.GetMin().y));
        }
    }
    return subImage;
}

template<class T>
void G_ImageG<T>::Copy(const G_ImageG<T> &image, const Vector2i &pos)
{
    Copy(image, Recti(pos, pos + image.GetSize()));
}

template<class T>
void G_ImageG<T>::Copy(const G_ImageG<T> &image,
                       const Recti &dstRect,
                       ImageResizeMode resizeMode)
{
    G_ImageG<T> resizedImage = image;

    Vector2i dstSize = dstRect.GetSize();
    resizedImage.Resize(dstSize, resizeMode);
    for (int y = 0; y < dstSize.y; ++y)
    {
        for (int x = 0; x < dstSize.x; ++x)
        {
            SetPixel(dstRect.GetMin().x + x,
                     dstRect.GetMin().y + y,
                     resizedImage.GetPixel(x, y));
        }
    }
}

template<class T>
void G_ImageG<T>::Copy(const G_ImageG<T> &image,
                       const Recti &srcCopyRect,
                       const Recti &dstCopyRect,
                       ImageResizeMode resizeMode)
{
    G_ImageG<T> subImageSrc = image.GetSubImage(srcCopyRect);
    subImageSrc.Resize(dstCopyRect.GetSize(), resizeMode);
    Copy(subImageSrc, dstCopyRect);
}

template<class T>
void G_ImageG<T>::Resize(const Vector2i &newSize,
                         ImageResizeMode resizeMode,
                         ImageAspectRatioMode arMode)
{
    Resize(newSize.x, newSize.y, resizeMode, arMode);
}

template<class T>
void G_ImageG<T>::Resize(const int _newWidth, int _newHeight,
                         ImageResizeMode resizeMode,
                         ImageAspectRatioMode arMode)
{
    int newWidth = _newWidth, newHeight = _newHeight;

    // First pick the new (width,height), depending on the AspectRatioMode
    if (arMode != ImageAspectRatioMode::Ignore)
    {
        Vector2 aspectRatio(SCAST<float>(newWidth)  / GetWidth(),
                            SCAST<float>(newHeight) / GetHeight());

        bool keepExc = (arMode == ImageAspectRatioMode::KeepExceeding);
        float ar =  (aspectRatio.x < aspectRatio.y) ?
                    (keepExc ? aspectRatio.y : aspectRatio.x) :
                    (keepExc ? aspectRatio.x : aspectRatio.y);
        newHeight = Math::Round(GetHeight() * ar);
        newWidth  = Math::Round(GetWidth()  * ar);
    }

    if (newWidth == GetWidth() && newHeight == GetHeight()) { return; }

    // Now do the resizing
    G_ImageG<T> original = *this;

    Vector2 sizeProp(original.GetWidth()  / SCAST<float>(newWidth),
                     original.GetHeight() / SCAST<float>(newHeight));

    Create(newWidth, newHeight);
    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < newWidth; ++x)
        {
            Color newColor;
            if (resizeMode == ImageResizeMode::Nearest)
            {
                // Pick nearest original pixel
                Vector2 oriCoord = Vector2(x,y) * sizeProp;
                int nearestX = Math::Round(oriCoord.x);
                int nearestY = Math::Round(oriCoord.y);
                newColor = original.GetPixel(nearestX, nearestY);
            }
            else
            {
                // Average all the original pixels mapping to this resized px
                Vector2 oriTopLeftF  = Vector2(  x,   y) * sizeProp;
                Vector2 oriBotRightF = Vector2(x+1, y+1) * sizeProp;
                Vector2i oriTopLeft(Math::Floor(oriTopLeftF.x),
                                    Math::Floor(oriTopLeftF.y));
                oriTopLeft = Vector2i::Max(oriTopLeft, Vector2i::Zero);
                Vector2i oriBotRight(Math::Ceil(oriBotRightF.x),
                                     Math::Ceil(oriBotRightF.y));
                oriBotRight = Vector2i::Min(oriBotRight, original.GetSize());

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

template<class T>
T *G_ImageG<T>::GetData() { return &m_pixels[0]; }

template<class T>
const T *G_ImageG<T>::GetData() const { return &m_pixels[0]; }

template<class T>
void G_ImageG<T>::SetPixel(int x, int y, const Color& color)
{
    ASSERT_MSG(false, "Please specialize this method!");
}

template<class T>
Color G_ImageG<T>::GetPixel(int x, int y) const
{
    ASSERT_MSG(false, "Please specialize this method!");
    return Color::Zero;
}

template<class T>
int G_ImageG<T>::GetWidth() const { return m_size.x; }

template<class T>
int G_ImageG<T>::GetHeight() const { return m_size.y; }

template<class T>
const Vector2i& G_ImageG<T>::GetSize() const { return m_size; }

template<class T>
void G_ImageG<T>::InvertVertically()
{
    G_ImageG<T> img = *this;
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            Color c = img.GetPixel(x, y);
            SetPixel(x, GetHeight() - y - 1, c);
        }
    }
}

template<class T>
G_ImageG<T> G_ImageG<T>::LoadFromData(int width, int height,
                              const Array<T> &rgbaByteData)
{
    G_ImageG<T> img(width, height);
    img.m_pixels = rgbaByteData;
    ASSERT(rgbaByteData.Size() == (img.GetWidth() * img.GetHeight() * 4));
    return img;
}

template<class T>
template<class OtherT>
G_ImageG<OtherT> G_ImageG<T>::To() const
{
    G_ImageG<OtherT> otherImg(GetWidth(), GetHeight());
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            otherImg.SetPixel(x, y, GetPixel(x,y));
        }
    }
    return otherImg;
}


template<class T>
void G_ImageG<T>::Export(const Path &filepath) const
{
    ASSERT_MSG(false, "Please implement this method!");
}

template<>
void G_ImageG<Byte>::Export(const Path &filepath) const
{
    ImageIO::Export(filepath, *this);
}

template<>
void G_ImageG<float>::Export(const Path &filepath) const
{
    G_ImageG<Byte> byteImg = this->To<Byte>();
    ImageIO::Export(filepath, byteImg);
}

template<class T>
void G_ImageG<T>::Import(const Path &imageFilepath)
{
    ASSERT_MSG(false, "Please implement this method!");
}

template<>
void G_ImageG<Byte>::Import(const Path &imageFilepath)
{
    bool ok; ImageIO::Import(imageFilepath, this, &ok);
}

template<>
void G_ImageG<float>::Import(const Path &imageFilepath)
{
    bool ok;
    G_ImageG<Byte> byteImg;
    ImageIO::Import(imageFilepath, &byteImg, &ok);
    *this = byteImg.To<float>();
}

// Specializations
template<>
void G_ImageG<Byte>::SetPixel(int x, int y, const Color &color)
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = SCAST<Byte>(color.r * 255);
    m_pixels[coord + 1] = SCAST<Byte>(color.g * 255);
    m_pixels[coord + 2] = SCAST<Byte>(color.b * 255);
    m_pixels[coord + 3] = SCAST<Byte>(color.a * 255);
}

template<>
void G_ImageG<float>::SetPixel(int x, int y, const Color &color)
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = color.r;
    m_pixels[coord + 1] = color.g;
    m_pixels[coord + 2] = color.b;
    m_pixels[coord + 3] = color.a;
}

template<>
Color G_ImageG<Byte>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0] / 255.0f,
                 m_pixels[coord + 1] / 255.0f,
                 m_pixels[coord + 2] / 255.0f,
                 m_pixels[coord + 3] / 255.0f);
}

template<>
Color G_ImageG<float>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0],
                 m_pixels[coord + 1],
                 m_pixels[coord + 2],
                 m_pixels[coord + 3]);
}

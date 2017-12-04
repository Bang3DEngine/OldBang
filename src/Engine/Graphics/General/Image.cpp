#include "Bang/Image.h"

#include "Bang/Rect.h"
#include "Bang/Debug.h"
#include "Bang/ImageIO.h"
#include "Bang/ImportFilesManager.h"

NAMESPACE_BANG_BEGIN

template<class T>
Image<T>::Image()
{

}

template<class T>
Image<T>::Image(int width, int height)
{
    Create(width, height);
}

template<class T>
void Image<T>::Create(int width, int height)
{
    m_size = Vector2i(width, height);
    m_pixels.Resize(m_size.x * m_size.y * 4);
}

template<class T>
void Image<T>::Create(int width, int height, const Color &backgroundColor)
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
Image<T> Image<T>::GetSubImage(const Recti &subCoords) const
{
    Vector2i subSize = subCoords.GetSize();
    Image<T> subImage(subSize.x, subSize.y);
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
void Image<T>::Copy(const Image<T> &image, const Vector2i &pos)
{
    Copy(image, Recti(pos, pos + image.GetSize()));
}

template<class T>
void Image<T>::Copy(const Image<T> &image,
                    const Recti &dstRect,
                    ImageResizeMode resizeMode)
{
    Image<T> resizedImage = image;

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
void Image<T>::Copy(const Image<T> &image,
                    const Recti &srcCopyRect,
                    const Recti &dstCopyRect,
                    ImageResizeMode resizeMode)
{
    Image<T> subImageSrc = image.GetSubImage(srcCopyRect);
    subImageSrc.Resize(dstCopyRect.GetSize(), resizeMode);
    Copy(subImageSrc, dstCopyRect);
}

template<class T>
void Image<T>::AddMargins(const Vector2i &margins,
                          const Color &marginColor,
                          AspectRatioMode arMode)
{
    Image<T> original = *this;

    Vector2i newSize = AspectRatio::GetAspectRatioedSize(
                                (margins * 2) + GetSize(), GetSize(), arMode);
    Create(newSize.x, newSize.y, marginColor);
    Copy(original,
         Recti(Vector2i::Zero, original.GetSize()),
         Recti(newSize / 2 -  original.GetSize() / 2,
               newSize / 2 + (original.GetSize()+1) / 2));
}

template<class T>
void Image<T>::AddMarginsToMatchAspectRatio(const Vector2i &arSizes,
                                            const Color &marginColor)
{
    AddMarginsToMatchAspectRatio(arSizes.x / float(arSizes.y), marginColor);
}

template<class T>
void Image<T>::AddMarginsToMatchAspectRatio(float aspectRatio,
                                            const Color &marginColor)
{
    Vector2i newSize = GetSize();
    if (aspectRatio > 1.0f) { newSize.x = (GetHeight() * aspectRatio); }
    else { newSize.y = GetWidth() / aspectRatio; }
    Vector2i margins = (newSize - GetSize());
    AddMargins(margins/2, marginColor, AspectRatioMode::Ignore);
}

template<class T>
void Image<T>::ResizeToMatchAspectRatio(const Vector2i &arSizes,
                                        bool makeBigger,
                                        ImageResizeMode resizeMode)
{
    ResizeToMatchAspectRatio(arSizes.x / float(arSizes.y), makeBigger, resizeMode);
}

template<class T>
void Image<T>::ResizeToMatchAspectRatio(float aspectRatio,
                                        bool makeBigger,
                                        ImageResizeMode resizeMode)
{
    Vector2i newSize = GetSize();
    bool modifyWidth = ((aspectRatio > 1.0f) == makeBigger);
    if (modifyWidth) { newSize.x = (GetHeight() * aspectRatio); }
    else { newSize.y = GetWidth() / aspectRatio; }
    Resize(newSize, resizeMode, AspectRatioMode::Ignore);
}

template<class T>
float Image<T>::GetAspectRatio() const
{
    return GetWidth() / Cast<float>(Math::Max(GetHeight(), 1));
}

template<class T>
void Image<T>::Resize(const Vector2i &newSize,
                      ImageResizeMode resizeMode,
                      AspectRatioMode arMode)
{
    Resize(newSize.x, newSize.y, resizeMode, arMode);
}

template<class T>
void Image<T>::Resize(int _newWidth, int _newHeight,
                      ImageResizeMode resizeMode,
                      AspectRatioMode arMode)
{
    // First pick the new (width,height), depending on the AspectRatioMode
    Vector2i newSize = AspectRatio::GetAspectRatioedSize(
                                            Vector2i(_newWidth, _newHeight),
                                            GetSize(), arMode);
    if (newSize.x == GetWidth() && newSize.y == GetHeight()) { return; }

    // Now do the resizing
    Image<T> original = *this;

    Vector2 sizeProp(original.GetWidth()  / Cast<float>(newSize.x),
                     original.GetHeight() / Cast<float>(newSize.y));

    Create(newSize.x, newSize.y);
    for (int y = 0; y < newSize.y; ++y)
    {
        for (int x = 0; x < newSize.x; ++x)
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
void Image<T>::FillTransparentPixels(const Color &color)
{
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            if (GetPixel(x,y).a == 0) { SetPixel(x,y,color); }
        }
    }
}

template<class T>
T *Image<T>::GetData() { return &m_pixels[0]; }

template<class T>
const T *Image<T>::GetData() const { return &m_pixels[0]; }

template<class T>
void Image<T>::SetPixel(int x, int y, const Color& color)
{
    ASSERT_MSG(false, "Please specialize this method!");
}

template<class T>
Color Image<T>::GetPixel(int x, int y) const
{
    ASSERT_MSG(false, "Please specialize this method!");
    return Color::Zero;
}

template<class T>
int Image<T>::GetWidth() const { return m_size.x; }

template<class T>
int Image<T>::GetHeight() const { return m_size.y; }

template<class T>
const Vector2i& Image<T>::GetSize() const { return m_size; }

template<class T>
void Image<T>::InvertVertically()
{
    Image<T> img = *this;
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
Image<T> Image<T>::LoadFromData(int width, int height,
                                const Array<T> &rgbaByteData)
{
    Image<T> img(width, height);
    img.m_pixels = rgbaByteData;
    ASSERT(rgbaByteData.Size() == (img.GetWidth() * img.GetHeight() * 4));
    return img;
}

template<class T>
template<class OtherT>
Image<OtherT> Image<T>::To() const
{
    Image<OtherT> otherImg(GetWidth(), GetHeight());
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
void Image<T>::Export(const Path &filepath) const
{
    ASSERT_MSG(false, "Please implement this method!");
}

template<>
void Image<Byte>::Export(const Path &filepath) const
{
    ImageIO::Export(filepath, *this);
}

template<>
void Image<float>::Export(const Path &filepath) const
{
    Image<Byte> byteImg = this->To<Byte>();
    ImageIO::Export(filepath, byteImg);
}

template<class T>
void Image<T>::Import(const Path &imageFilepath)
{
    ASSERT_MSG(false, "Please implement this method!");
}

template<>
void Image<Byte>::Import(const Path &imageFilepath)
{
    bool ok; ImageIO::Import(imageFilepath, this, &ok);
}

template<>
void Image<float>::Import(const Path &imageFilepath)
{
    bool ok;
    Image<Byte> byteImg;
    ImageIO::Import(imageFilepath, &byteImg, &ok);
    *this = byteImg.To<float>();
}

// Specializations
template<>
void Image<Byte>::SetPixel(int x, int y, const Color &color)
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = Cast<Byte>(color.r * 255);
    m_pixels[coord + 1] = Cast<Byte>(color.g * 255);
    m_pixels[coord + 2] = Cast<Byte>(color.b * 255);
    m_pixels[coord + 3] = Cast<Byte>(color.a * 255);
}

template<>
void Image<float>::SetPixel(int x, int y, const Color &color)
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
Color Image<Byte>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const std::size_t coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0] / 255.0f,
                 m_pixels[coord + 1] / 255.0f,
                 m_pixels[coord + 2] / 255.0f,
                 m_pixels[coord + 3] / 255.0f);
}

template<>
Color Image<float>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const std::size_t coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0],
                 m_pixels[coord + 1],
                 m_pixels[coord + 2],
            m_pixels[coord + 3]);
}

// ImageResource
template<class T>
ImageResource<T>::ImageResource() : Image<T>()
{

}

template<class T>
ImageResource<T>::ImageResource(int width, int height) : Image<T>(width, height)
{

}

template<class T>
void ImageResource<T>::Import(const Path &imageFilepath)
{
    Image<T>::Import(imageFilepath);
}

NAMESPACE_BANG_END

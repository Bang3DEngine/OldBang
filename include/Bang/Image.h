#ifndef IMAGE_H
#define IMAGE_H

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Resource.h"

enum class ImageAspectRatioMode {Ignore, Keep, KeepExceeding};
enum class ImageResizeMode {Nearest, Linear};

template<class T>
class Image : public Resource
{
    RESOURCE(Image)

public:
    Image();
    Image(int width, int height);

    void Create(int width, int height);
    void Create(int width, int height, const Color& backgroundColor);
    void SetPixel(int x, int y, const Color& color);

    Image<T> GetSubImage(const Recti &subImageCoordsPx) const;
    void Copy(const Image<T> &image, const Vector2i& pos);
    void Copy(const Image<T> &image,
              const Recti& dstRect,
              ImageResizeMode resizeMode = ImageResizeMode::Linear);
    void Copy(const Image<T> &image,
              const Recti& srcCopyRect,
              const Recti& dstCopyRect,
              ImageResizeMode resizeMode = ImageResizeMode::Linear);

    void AddMargins(const Vector2i& margins,
                    const Color &marginColor = Color::Zero,
                    ImageAspectRatioMode arMode = ImageAspectRatioMode::Ignore);

    void AddMarginsToMatchAspectRatio(const Vector2i &aspectRatioSizes,
                                      const Color &marginColor = Color::Zero);
    void AddMarginsToMatchAspectRatio(float aspectRatio,
                                      const Color &marginColor = Color::Zero);

    void ResizeToMatchAspectRatio(
               const Vector2i &aspectRatioSizes,
               bool makeBigger = false,
               ImageResizeMode resizeMode = ImageResizeMode::Linear);
    void ResizeToMatchAspectRatio(
            float aspectRatio,
            bool makeBigger = false,
            ImageResizeMode resizeMode = ImageResizeMode::Linear);

    float GetAspectRatio() const;

    void Resize(const Vector2i &newSize,
                ImageResizeMode resizeMode = ImageResizeMode::Linear,
                ImageAspectRatioMode arMode = ImageAspectRatioMode::Ignore);
    void Resize(int newWidth, int newHeight,
                ImageResizeMode resizeMode = ImageResizeMode::Linear,
                ImageAspectRatioMode arMode = ImageAspectRatioMode::Ignore);

    void FillTransparentPixels(const Color &color);

    T* GetData();
    const T* GetData() const;
    Color GetPixel(int x, int y) const;
    int GetWidth() const;
    int GetHeight() const;
    const Vector2i& GetSize() const;

    void InvertVertically();

    template<class OtherT>
    Image<OtherT> To() const;
    void Export(const Path &filepath) const;
    static Image<T> LoadFromData(int width, int height,
                                    const Array<T> &rgbaByteData);

    // Resource
    virtual void Import(const Path &imageFilepath) override;

private:
    Vector2i m_size = Vector2i::Zero;
    Array<T> m_pixels;

    static Vector2i GetAspectRatioedSize(const Vector2i& targetSize,
                                         const Vector2i& currentSize,
                                         ImageAspectRatioMode aspectRatioMode);
};

template class Image<Byte>;
template class Image<float>;

using Imageb = Image<Byte>;
using Imagef = Image<float>;

#endif // IMAGE_H

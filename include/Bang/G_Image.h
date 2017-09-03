#ifndef IMAGE_H
#define IMAGE_H

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Resource.h"

class G_Image : public Resource
{
    RESOURCE(G_Image)

public:
    enum class ResizeMode { Nearest, Linear };

    G_Image();
    G_Image(int width, int height);

    void Create(int width, int height);
    void Create(int width, int height, const Color& backgroundColor);
    void SetPixel(int x, int y, const Color& color);

    G_Image GetSubImage(const Recti &subImageCoordsPx) const;
    void Copy(const G_Image &image, const Vector2i& pos);
    void Copy(const G_Image &image,
              const Recti& dstRect,
              ResizeMode resizeMode = ResizeMode::Linear);
    void Copy(const G_Image &image,
              const Recti& srcCopyRect,
              const Recti& dstCopyRect,
              ResizeMode resizeMode = ResizeMode::Linear);

    void Resize(const Vector2i &newSize,
                ResizeMode resizeMode = ResizeMode::Linear);
    void Resize(const int newWidth, int newHeight,
                ResizeMode resizeMode = ResizeMode::Linear);

    Byte* GetData();
    const Byte* GetData() const;
    Color GetPixel(int x, int y) const;
    int GetWidth() const;
    int GetHeight() const;
    const Vector2i& GetSize() const;

    void InvertVertically();

    void SaveToFile(const Path &filepath) const;
    static G_Image LoadFromData(int width, int height,
                                const Array<Byte> &rgbaByteData);

    // Resource
    virtual void Import(const Path &imageFilepath) override;

private:
    Vector2i m_size;
    Array<Byte> m_pixels;
};

#endif // IMAGE_H

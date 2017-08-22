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
    G_Image();
    G_Image(int width, int height);

    void Create(int width, int height);
    void Create(int width, int height, const Color& backgroundColor);
    void SetPixel(int x, int y, const Color& color);

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
    static G_Image LoadFromFile(const Path &filepath);

private:
    Vector2i m_size;
    Array<Byte> m_pixels;
};

#endif // IMAGE_H

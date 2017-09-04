#ifndef G_TEXTURE2D_H
#define G_TEXTURE2D_H

#include "Bang/G_Image.h"
#include "Bang/G_Texture.h"

class G_Texture2D : public G_Texture
{
public:
    G_Texture2D();
    virtual ~G_Texture2D();

    void Import(const G_ImageG<Byte> &image);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(const Color &fillColor,
              int width, int height,
              bool genMipMaps = true);
    void Fill(const Byte *newData,
              int width, int height,
              GL::ColorComp inputDataColorComp,
              GL::DataType inputDataType,
              bool genMipMaps = true);
    void GenerateMipMaps() const;

    template<class T = Byte>
    G_ImageG<T> ToImage(bool invertY = false)
    {
        const int width  = GetWidth(), height = GetHeight();
        const int numComps = GL::GetNumComponents(GetInternalFormat());
        T *pixels = new T[width * height * numComps];

        GLId prevBound = GL::GetBoundId(GL::BindTarget::Texture2D);
        Bind();
        GL::GetTexImage(GetTextureTarget(), pixels);
        GL::Bind(GL::BindTarget::Texture2D, prevBound);

        G_ImageG<T> img(width, height);
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const int coords = (y * width + x) * 4;
                Color pixelColor = GetColorFromArray(pixels, coords);
                img.SetPixel(x, y, pixelColor);
            }
        }
        if (invertY) { img.InvertVertically(); }

        delete[] pixels;

        return img;
    }

    void SetAlphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

protected:
    float m_alphaCutoff = 0.1f;

private:
    static Color GetColorFromArray(const float *pixels, int i);
    static Color GetColorFromArray(const Byte *pixels, int i);
};

#endif // G_TEXTURE2D_H

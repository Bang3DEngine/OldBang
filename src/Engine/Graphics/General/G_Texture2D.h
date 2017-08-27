#ifndef G_TEXTURE2D_H
#define G_TEXTURE2D_H

#include "Bang/G_Image.h"
#include "Bang/G_Texture.h"

class G_Texture2D : public G_Texture
{
public:
    G_Texture2D();
    virtual ~G_Texture2D();

    void LoadFromImage(const G_Image &image);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(const Byte *newData, int width, int height,
              G_Texture::Format imageFormat,
              bool genMipMaps = true);
    void Fill(const Byte *newData, int width, int height,
              int sizeOfNewData, bool genMipMaps = true);
    void GenerateMipMaps() const;

    G_Image ToImage(bool invertY = false);

    void SetAlphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

protected:
    float m_alphaCutoff = 0.1f;
};

#endif // G_TEXTURE2D_H

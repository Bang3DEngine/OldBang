#ifndef G_TEXTURE2D_H
#define G_TEXTURE2D_H

#include "Bang/Array.h"
#include "Bang/G_Image.h"
#include "Bang/G_Texture.h"

class G_Texture2D : public G_Texture
{
public:
    G_Texture2D();
    G_Texture2D(const Path &imageFilepath);
    virtual ~G_Texture2D();

    void LoadFromImage(const Path &imageFilepath);
    void LoadFromImage(const G_Image &image);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(const byte *newData, int width, int height,
              G_Texture::Format imageFormat,
              bool genMipMaps = true);
    void Fill(const byte *newData, int width, int height,
              int sizeOfNewData, bool genMipMaps = true);
    void GenerateMipMaps() const;

    const Path& GetImageFilepath() const;

    G_Image ToImage(bool invertY = false);

    void SetAlphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

protected:
    G_Image m_image;
    Path m_imageFilepath;
    float m_alphaCutoff = 0.1f;

private:
    G_Texture2D(const G_Texture2D &t); // No copy constr.
};

#endif // G_TEXTURE2D_H

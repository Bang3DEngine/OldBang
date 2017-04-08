#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Bang/Image.h"
#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/Texture.h"

class Texture2D : public Texture, public Asset
{
	OBJECT(Texture2D)
    ASSET_ICON(Texture2D, "Icons/ImageIcon.png")

public:
    Texture2D();
    Texture2D(const String &imageFilepath);
    virtual ~Texture2D();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    void LoadFromImage(const String &imageFilepath);
    void LoadFromImage(const Image &image);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(const unsigned char *newData, int width, int height,
              Texture::Format imageFormat,
              bool genMipMaps = true);
    void Fill(const unsigned char *newData, int width, int height,
              int sizeOfNewData, bool genMipMaps = true);
    void GenerateMipMaps() const;

    String GetImageFilepath() const;

    void SetAlphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    Image m_image;
    String m_imageFilepath = "";
    float m_alphaCutoff = 0.1f;

    Texture2D(const Texture2D &t); // No copy constr.
};

#endif // TEXTURE2D_H

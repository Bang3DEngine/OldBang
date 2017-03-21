#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Array.h"
#include "Asset.h"
#include "Texture.h"

class Texture2D : public Texture, public Asset
{
    ASSET_ICON(Texture2D, "Icons/ImageIcon.png")

public:
    Texture2D();
    Texture2D(const String &imageFilepath);
    virtual ~Texture2D();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    void LoadFromImage(const String &imageFilepath);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height,
              Texture::Format imageFormat,
              bool genMipMaps = true);
    void Fill(unsigned char *newData, int width, int height,
              int sizeOfNewData, bool genMipMaps = true);

    String GetImageFilepath() const;

    void SetAlphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:

    String m_imageFilepath = "";
    float m_alphaCutoff = 0.1f;

    Texture2D(const Texture2D &t); // No copy constr.
};

#endif // TEXTURE2D_H

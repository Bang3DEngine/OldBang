#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Array.h"

#include "Asset.h"
#include "Texture.h"

class Texture2D : public Texture,
                  public Asset
{
public:

    Texture2D();
    Texture2D(const String &imageFilepath);
    virtual ~Texture2D();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

    void LoadFromFile(const String &imageFilepath);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height, bool genMipMaps = true);

    String GetImageRelativeFilepath() const;

    void SetalphaCutoff(float alphaCutoff);
    float GetAlphaCutoff() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

private:

    float m_alphaCutoff = 0.1f;
    Texture2D(const Texture2D &t); // No copy constr.
};

#endif // TEXTURE2D_H

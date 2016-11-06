#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Bang.h"


#include "Array.h"
#include <stack>

#include "glm/glm.hpp"

#include "FileReader.h"
#include "Texture.h"
#include "Debug.h"
#include "Asset.h"

class Texture2D : public Texture,
                  public Asset
{

private:

    float m_alphaCuttoff = 0.1f;

    Texture2D(const Texture2D &t) : Texture2D() {}

public:
    const static String GetFileExtensionStatic() { return "btex2d"; }
    const virtual String GetFileExtension()
    {
        return Texture2D::GetFileExtensionStatic();
    }


public:
    Texture2D();
    Texture2D(const String &imageFilepath);
    virtual ~Texture2D();

    void LoadFromFile(const String &imageFilepath);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height, bool genMipMaps = true);

    String GetImageRelativeFilepath() const;

    void SetAlphaCuttoff(float alphaCuttoff);
    float GetAlphaCuttoff() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // TEXTURE2D_H

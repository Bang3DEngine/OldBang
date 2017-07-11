#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/G_Texture2D.h"

class Texture2D : public G_Texture2D,
                  public Asset
{
	OBJECT(Texture2D)

public:
    Texture2D();
    Texture2D(const Path &imageFilepath);
    explicit Texture2D(const G_Texture2D *gTexture);
    virtual ~Texture2D();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // TEXTURE2D_H

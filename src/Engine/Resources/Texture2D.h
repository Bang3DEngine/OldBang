#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Bang/Asset.h"
#include "Bang/G_Texture2D.h"

class Texture2D : public G_Texture2D,
                  public Asset
{
    ASSET(Texture2D)

public:
    Texture2D();
    Texture2D(const Path &imageFilepath);
    explicit Texture2D(const G_Texture2D *gTexture);
    virtual ~Texture2D();

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // Resource
    virtual void Import(const Path &imageFilepath) override;
};

#endif // TEXTURE2D_H

#ifndef FONT_H
#define FONT_H

#include "Bang/Asset.h"
#include "Bang/G_Font.h"

class Font : public G_Font,
             public Asset
{
    ASSET(Font)

public:
    Font();
    virtual ~Font();

    Path GetTTFFilepath() const;

    // Resource
    void Import(const Path &ttfFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Path m_ttfFilepath;
};

#endif // FONT_H

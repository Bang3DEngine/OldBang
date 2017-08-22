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

    void LoadFromTTF(const Path &ttfFilepath) override;
    Path GetTTFFilepath() const;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    Path m_ttfFilepath;
};

#endif // FONT_H

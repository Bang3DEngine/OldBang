#ifndef FONT_H
#define FONT_H

#include "Bang.h"

#include <GL/glew.h>
#include "Array.h"

#include "glm/glm.hpp"

#include "Texture2D.h"
#include "Asset.h"

class Texture2D;
class Font : public Asset
{
public:
    const static String GetFileExtensionStatic() { return "bfont"; }
    const virtual String GetFileExtension()
    {
        return Font::GetFileExtensionStatic();
    }

    Font();
    virtual ~Font();

    void LoadFromFile(const String &m_filepath);

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    Texture2D *GetFontSheet() const;

private:
    Texture2D *m_fontsheet = nullptr;
};

#endif // FONT_H

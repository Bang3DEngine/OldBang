#ifndef TEXTFILEINSPECTABLE_H
#define TEXTFILEINSPECTABLE_H

#include "TextFile.h"
#include "IInspectable.h"

class XMLNode;
class TextFileInspectable : public IInspectable
{
private:
    TextFile m_textFile;

public:
    TextFileInspectable(const TextFile &textFile);

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    #endif
};

#endif // TEXTFILEINSPECTABLE_H

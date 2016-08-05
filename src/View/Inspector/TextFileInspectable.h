#ifndef TEXTFILEINSPECTABLE_H
#define TEXTFILEINSPECTABLE_H

#include "TextFile.h"
#include "FileWriter.h"
#include "IInspectable.h"

class TextFileInspectable : public IInspectable
{
private:
    TextFile m_textFile;

public:
    TextFileInspectable(const TextFile &textFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // TEXTFILEINSPECTABLE_H

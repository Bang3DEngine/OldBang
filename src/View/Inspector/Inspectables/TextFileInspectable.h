#ifndef TEXTFILEINSPECTABLE_H
#define TEXTFILEINSPECTABLE_H

#include "TextFile.h"
#include "SerializableObject.h"

class XMLNode;
class TextFileInspectable : public SerializableObject
{
private:
    TextFile m_textFile;

public:
    TextFileInspectable(const TextFile &textFile);

    #ifdef BANG_EDITOR
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    #endif
};

#endif // TEXTFILEINSPECTABLE_H

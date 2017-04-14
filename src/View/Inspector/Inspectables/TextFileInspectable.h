#ifndef TEXTFILEINSPECTABLE_H
#define TEXTFILEINSPECTABLE_H

#include "Bang/TextFile.h"
#include "Bang/SerializableObject.h"

class XMLNode;
class TextFileInspectable : public SerializableObject
{
    OBJECT(TextFileInspectable)

public:
    TextFileInspectable();
    TextFileInspectable(const TextFile &textFile);

    #ifdef BANG_EDITOR
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    #endif

private:
    TextFile m_textFile;
};

#endif // TEXTFILEINSPECTABLE_H

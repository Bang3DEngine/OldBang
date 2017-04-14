#include "Bang/TextFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"

TextFileInspectable::TextFileInspectable()
{
}

TextFileInspectable::TextFileInspectable(const TextFile &textFile) :
    m_textFile(textFile)
{
}

#ifdef BANG_EDITOR
void TextFileInspectable::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    String contents = xmlInfo.GetString("Contents");
    IO::WriteToFile(m_textFile.GetAbsolutePath(), contents);
}

void TextFileInspectable::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
    xmlInfo->SetTagName("Text file");
    xmlInfo->SetString("Contents", m_textFile.GetContents(),
                      {XMLProperty::BigText});
}
#endif

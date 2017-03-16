#include "TextFileInspectable.h"

#include "IO.h"
#include "XMLNode.h"

TextFileInspectable::TextFileInspectable(const TextFile &textFile) :
    m_textFile(textFile)
{
}

#ifdef BANG_EDITOR
void TextFileInspectable::Read(const XMLNode &xmlInfo)
{
    String contents = xmlInfo.GetString("Contents");
    IO::WriteToFile(m_textFile.GetAbsolutePath(), contents);
}

void TextFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Text file");
    xmlInfo->SetString("Contents", m_textFile.GetContents(),
                      {XMLProperty::BigText});
}
#endif

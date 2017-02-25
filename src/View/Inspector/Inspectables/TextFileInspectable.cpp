#include "TextFileInspectable.h"

#include "XMLNode.h"
#include "Persistence.h"

TextFileInspectable::TextFileInspectable(const TextFile &textFile) :
    m_textFile(textFile)
{

}

#ifdef BANG_EDITOR
void TextFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    String contents = xmlInfo->GetString("Contents");
    Persistence::WriteToFile(m_textFile.GetAbsolutePath(), contents);
}

void TextFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Text file");
    xmlInfo->SetString("Contents", m_textFile.GetContents(),
                      {XMLProperty::BigText});
}
#endif

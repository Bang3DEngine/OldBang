#include "TextFileInspectable.h"

TextFileInspectable::TextFileInspectable(const TextFile &textFile) :
    m_textFile(textFile)
{

}

void TextFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    String contents = xmlInfo->GetString("Contents");
    FileWriter::WriteToFile(m_textFile.GetRelativePath(), contents);
}

void TextFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Text file");

    xmlInfo->SetString("Contents", m_textFile.GetContents(), {XMLProperty::BigText});
}

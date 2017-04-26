#include "Bang/TextFile.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/Inspectable.h"
#include "Bang/FileInspectable.h"

TextFile::TextFile()
{
}

TextFile::TextFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    m_contents = IO::GetFileContents(m_path);
}

String TextFile::GetContents() const
{
    return m_contents;
}

void TextFile::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    String contents = xmlInfo.GetString("Contents");
    IO::WriteToFile(GetAbsolutePath(), contents);
}

void TextFile::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
    xmlInfo->SetTagName("Text file");
    xmlInfo->SetString("Contents", GetContents(), {XMLProperty::BigText});
}

#ifdef BANG_EDITOR
IInspectable *TextFile::GetNewInspectable()
{
    return new FileInspectable<TextFile>(*this);
}
#endif

bool TextFile::IsAsset() const
{
    return false;
}

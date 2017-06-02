#include "Bang/TextFile.h"

#include "Bang/Paths.h"
#include "Bang/FileReader.h"
#include "Bang/Inspectable.h"
#include "Bang/FileInspectable.h"

TextFile::TextFile()
{
}

TextFile::TextFile(const Path& path)
    : BFile(path)
{
    m_contents = File::GetContents( GetPath() );
}

String TextFile::GetContents() const
{
    return m_contents;
}

void TextFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
    String contents = xmlInfo.GetString("Contents");
    File::Write(GetPath(), contents);
}

void TextFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

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

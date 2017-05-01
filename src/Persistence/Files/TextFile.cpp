#include "Bang/TextFile.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/Inspectable.h"
#include "Bang/FileInspectable.h"

TextFile::TextFile()
{
}

TextFile::TextFile(const Path& path)
    : File(path)
{
    m_contents = IO::GetFileContents( GetPath() );
}

String TextFile::GetContents() const
{
    return m_contents;
}

void TextFile::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    String contents = xmlInfo.GetString("Contents");
    IO::WriteToFile( GetPath().GetAbsolute(), contents);
}

void TextFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

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

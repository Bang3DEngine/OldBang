#include "TextFile.h"

#include "FileReader.h"

#ifdef BANG_EDITOR
#include "TextFileInspectable.h"
#endif

TextFile::TextFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    m_contents = FileReader::GetContents(m_path);
}

String TextFile::GetContents() const
{
    return m_contents;
}

#ifdef BANG_EDITOR
IInspectable *TextFile::GetInspectable() const
{
    return new TextFileInspectable(*this);
}
#endif

bool TextFile::IsAsset() const
{
    return false;
}

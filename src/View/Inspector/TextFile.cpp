#include "TextFile.h"

#include "TextFileInspectable.h"

TextFile::TextFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    m_contents = FileReader::GetContents(m_path);
}

String TextFile::GetContents() const
{
    return m_contents;
}

IInspectable *TextFile::GetInspectable() const
{
    return new TextFileInspectable(*this);
}

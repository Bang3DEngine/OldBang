#include "TextFile.h"

TextFile::TextFile(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    m_contents = FileReader::GetContents(m_path);
}

std::string TextFile::GetContents() const
{
    return m_contents;
}

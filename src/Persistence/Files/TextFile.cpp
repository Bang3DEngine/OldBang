#include "Bang/TextFile.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/Inspectable.h"

#ifdef BANG_EDITOR
#include "Bang/TextFileInspectable.h"
#endif

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

#ifdef BANG_EDITOR
IInspectable *TextFile::GetNewInspectable() const
{
    return nullptr;
}
#endif

bool TextFile::IsAsset() const
{
    return false;
}

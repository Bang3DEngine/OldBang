#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <string>

#include "File.h"
#include "FileReader.h"

class TextFile : public File
{
protected:
    std::string m_contents = "";

public:
    TextFile(const QFileSystemModel *model, const QModelIndex *index);

    std::string GetContents() const;
};

#endif // TEXTFILE_H

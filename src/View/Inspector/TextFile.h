#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "File.h"
#include "FileReader.h"

class TextFile : public File
{
protected:
    String m_contents = "";

public:
    TextFile(const QFileSystemModel *model, const QModelIndex &index);

    String GetContents() const;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif
};

#endif // TEXTFILE_H

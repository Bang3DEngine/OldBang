#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>

class File
{
protected:

    bool isFile = true;
    std::string path = "", name = "", extension = "";

public:
    File(const QFileSystemModel *model, const QModelIndex *index);

    bool IsImage() const;
    bool IsMesh() const;

    bool IsDir() const;
    bool IsFile() const;
    const std::string& GetPath() const;
    const std::string& GetName() const;
    const std::string& GetExtension() const;
};

#endif // FILE_H

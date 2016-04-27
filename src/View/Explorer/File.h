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

    bool IsTexture2DAsset() const;
    bool IsImageFile() const;

    bool IsMeshAsset() const;
    bool IsMeshFile() const;

    bool IsMaterialAsset() const;

    bool IsPrefabAsset() const;

    bool IsDir() const;
    bool IsFile() const;

    const std::string& GetPath() const;
    const std::string& GetName() const;
    const std::string& GetExtension() const;
};

#endif // FILE_H

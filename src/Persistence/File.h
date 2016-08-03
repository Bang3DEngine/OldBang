#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>

class File
{
protected:

    bool m_isFile = true;
    std::string m_path = "";
    std::string m_name = "";
    std::string m_extension = "";

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

    const std::string& GetRelativePath() const;
    const std::string& GetName() const;
    const std::string& GetExtension() const;
    std::string GetNameAndExtension() const;
};

#endif // FILE_H

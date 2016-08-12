#ifndef FILE_H
#define FILE_H

#include <QPainter>
#include <QModelIndex>
#include <QFileSystemModel>

#include "IInspectable.h"

class File
{
protected:

    const QFileSystemModel *m_fileSystemModel = nullptr;
    QModelIndex m_modelIndex;

    bool m_isFile = true;
    std::string m_path = "";
    std::string m_name = "";
    std::string m_absPath = "";
    std::string m_extension = "";

    // Adds on top of the passed QPixmap the little icon that
    // distinguishes between assets and noAsset files.
    static QPixmap AddNoAssetFileQPixmapOnTopOf(const QPixmap &pm);

public:
    File();
    File(const std::string &filepath);
    File(const QFileSystemModel *model, const QModelIndex &index);
    virtual ~File();

    bool IsTexture2DAsset() const;
    bool IsImageFile() const;

    bool IsMeshAsset() const;
    bool IsMeshFile() const;

    bool IsMaterialAsset() const;
    bool IsBehaviour() const;

    bool IsTextFile() const;

    bool IsPrefabAsset() const;

    bool IsDir() const;
    bool IsFile() const;

    /**
     * @brief The input must be something like "jpg png bmp obj"
     * @param extensions
     * @return
     */
    bool IsOfExtension(const std::string &extensions) const;

    static File *GetSpecificFile(const File &f);
    virtual QPixmap GetIcon() const;
    virtual IInspectable *GetInspectable() const;

    std::string GetContents() const;

    const std::string& GetAbsolutePath() const;
    const std::string& GetRelativePath() const;
    const std::string& GetName() const;
    const std::string& GetExtension() const;
    std::string GetNameAndExtension() const;
};

#endif // FILE_H

#ifndef FILEMESHASSET_H
#define FILEMESHASSET_H

#include "FileReader.h"
#include "Logger.h"

#include "File.h"

class MeshAssetFile : public File
{
private:
    std::string m_meshFilepath = "";

public:
    MeshAssetFile(const QFileSystemModel *model, const QModelIndex *index);
    virtual QPixmap GetIcon() const override;
    virtual IInspectable *GetInspectable() const override;

    const std::string& GetMeshFilepath() const;
};

#endif // FILEMESHASSET_H

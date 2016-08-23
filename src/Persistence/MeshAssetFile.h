#ifndef FILEMESHASSET_H
#define FILEMESHASSET_H

#include "FileReader.h"
#include "Debug.h"

#include "File.h"

class MeshAssetFile : public File
{
private:
    String m_meshFilepath = "";

public:
    MeshAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual QPixmap GetIcon() const override;
    virtual IInspectable *GetInspectable() const override;

    const String& GetMeshFilepath() const;
};

#endif // FILEMESHASSET_H

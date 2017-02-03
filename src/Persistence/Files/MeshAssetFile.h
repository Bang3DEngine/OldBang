#ifndef FILEMESHASSET_H
#define FILEMESHASSET_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "File.h"
#include "String.h"
#include "FileReader.h"

class IInspectable;
class MeshAssetFile : public File
{
public:
    MeshAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif

    virtual bool IsAsset() const override;

    void SetMeshFilepath(const String &meshFilepath);
    const String& GetMeshFilepath() const;

private:
    String m_meshFilepath = "";
};

#endif // FILEMESHASSET_H

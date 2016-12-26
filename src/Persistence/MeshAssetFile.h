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
private:
    String m_meshFilepath = "";

public:
    MeshAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif

    const String& GetMeshFilepath() const;
};

#endif // FILEMESHASSET_H

#ifndef FILEMESHASSET_H
#define FILEMESHASSET_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/FileReader.h"

class SerializableObject;
class MeshAssetFile : public File
{
public:
    MeshAssetFile();
    MeshAssetFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

    void SetMeshFilepath(const String &meshFilepath);
    const String& GetMeshFilepath() const;

    void Read(const XMLNode &xmlInfo);
    void Write(XMLNode *xmlInfo) const;

private:
    String m_meshFilepath = "";
};

#endif // FILEMESHASSET_H

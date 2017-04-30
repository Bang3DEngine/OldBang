#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/XMLNode.h"

class SerializableObject;
class Texture2DAssetFile : public File
{
public:
    Texture2DAssetFile();
    Texture2DAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    Path GetImageFilepath() const;
    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

private:
    XMLNode m_xmlInfo;
};

#endif // FILETEXTURE2DASSET_H

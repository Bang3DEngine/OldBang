#ifndef BFILE_H
#define BFILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/SerializableObject.h"

class XMLNode;
class BFile : public File,
              public SerializableObject
{
public:
    BFile();
    BFile(const Path &filepath);
    BFile(const String &filepath);
    BFile(const QFileSystemModel *model, const QModelIndex &index);
    virtual ~BFile();

    virtual const QPixmap& GetIcon() const;

    bool IsSound() const;
    bool IsAudioClipFile() const;

    bool IsTexture2DFile() const;
    bool IsImageFile() const;

    bool IsScene() const;

    bool IsMeshFile() const;
    bool IsModelFile() const;

    bool IsMaterialFile() const;
    bool IsBehaviour() const;

    bool IsTextFile() const;
    bool IsFontFile() const;

    bool IsPrefabFile() const;
    bool IsShaderProgramFile() const;

    virtual bool IsAsset() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    static BFile *GetSpecificFile(const BFile &f);

    friend class FileSystemModel;
    friend class DialogBrowseAssetFile;
};

#endif // BFILE_H

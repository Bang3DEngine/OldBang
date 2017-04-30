#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/SerializableObject.h"

class XMLNode;
class IInspectable;
class File : public SerializableObject
{
public:
    File();
    File(const Path &filepath);
    File(const String &filepath);
    File(const QFileSystemModel *model, const QModelIndex &index);

    bool IsSound() const;
    bool IsAudioClipAsset() const;

    bool IsTexture2DAsset() const;
    bool IsImageFile() const;

    bool IsScene() const;

    bool IsMeshFile() const;
    bool IsModelFile() const;

    bool IsMaterialAsset() const;
    bool IsBehaviour() const;

    bool IsTextFile() const;
    bool IsFontAssetFile() const;

    bool IsPrefabAsset() const;
    bool IsShaderProgramAssetFile() const;

    static File *GetSpecificFile(const File &f);
    static bool Exists(const String &filepath);
    static void Write(const String &filepath, const String &contents);
    static void Write(const String &filepath, const Array<String> &lines);
    static void Write(const String &filepath, const List<String> &lines);
    static String GetContents(const String &filepath);

    virtual const QPixmap& GetIcon() const;

    void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable* GetNewInspectable();
    #endif

    String GetContents() const;
    const Path& GetPath() const;

    virtual bool IsAsset() const;

protected:
    Path m_path;

    friend class FileSystemModel;
    friend class DialogBrowseAssetFile;
};

#endif // FILE_H

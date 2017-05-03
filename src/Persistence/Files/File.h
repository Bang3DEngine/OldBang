#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include <QCryptographicHash>
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

    void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable* GetNewInspectable();
    #endif

    String GetContents() const;
    const Path& GetPath() const;

    virtual bool IsAsset() const;

    static bool Remove(const Path &path);
    static bool CreateDirectory(const Path &dirPath);
    static bool Move(const Path &oldPath, const Path &newPath);

    static bool Duplicate(const Path &fromPath, const Path &toPath);
    static bool DuplicateFile(const Path &fromFilepath,
                              const Path &toFilepath,
                              bool overwrite = true);
    static bool DuplicateDir(const Path &fromDirpath,
                             const Path &toDirpath,
                             bool overwrite = true);

    static File *GetSpecificFile(const File &f);

    static String GetContents(const Path &filepath);
    static void Write(const Path &filepath, const String &contents);
    static void Write(const Path &filepath, const Array<String> &lines);
    static void Write(const Path &filepath, const List<String> &lines);

    static String GetHash(const Path &filepath);
    static String GetHashFromString(const String &str);
    static String GetHashFromByteArray(const QByteArray &byteArray);

protected:
    Path m_path;

    friend class FileSystemModel;
    friend class DialogBrowseAssetFile;
};

#endif // FILE_H

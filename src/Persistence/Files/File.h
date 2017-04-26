#ifndef FILE_H
#define FILE_H

#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/String.h"
#include "Bang/SerializableObject.h"

class IInspectable;
class File : public SerializableObject
{
public:

    File();
    File(const String &filepath);
    File(const QFileSystemModel *model, const QModelIndex &index);

    virtual ~File();

    bool IsSound() const;
    bool IsAudioClipAsset() const;

    bool IsTexture2DAsset() const;
    bool IsImageFile() const;

    bool IsScene() const;

    bool IsMeshAsset() const;
    bool IsMeshFile() const;

    bool IsMaterialAsset() const;
    bool IsBehaviour() const;

    bool IsTextFile() const;
    bool IsFontAssetFile() const;

    bool IsPrefabAsset() const;
    bool IsShaderProgramAssetFile() const;

    bool IsDir() const;
    bool IsFile() const;

    /**
     * @brief The input must be something like "jpg png bmp obj"
     * @param extensions
     * @return
     */
    bool IsOfExtension(const String &extensions) const;

    static File *GetSpecificFile(const File &f);
    static bool Exists(const String &filepath);
    static void Write(const String &filepath, const String &contents);
    static void Write(const String &filepath, const Array<String> &lines);
    static void Write(const String &filepath, const List<String> &lines);
    static String GetContents(const String &filepath);

    virtual const QPixmap& GetIcon() const;

    #ifdef BANG_EDITOR
    virtual IInspectable* GetNewInspectable();
    #endif

    String GetContents() const;

    const String& GetAbsolutePath() const;
    const String& GetRelativePath() const;
    const String& GetName() const;
    const String& GetExtension() const;
    String GetNameAndExtension() const;

    virtual bool IsAsset() const;

protected:
    const QFileSystemModel *m_fileSystemModel = nullptr;
    QModelIndex m_modelIndex;

    bool m_isFile = true;
    String m_path = "";
    String m_name = "";
    String m_extension = "";

    friend class FileSystemModel;
    friend class DialogBrowseAssetFile;
};

#endif // FILE_H

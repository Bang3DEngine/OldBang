#include "File.h"

#include <fstream>
#include <QPainter>

#ifdef BANG_EDITOR
#include "Explorer.h"
#include "SerializableObject.h"
#endif

#include "IO.h"
#include "Font.h"
#include "Mesh.h"
#include "Scene.h"
#include "Prefab.h"
#include "Material.h"
#include "TextFile.h"
#include "MeshFile.h"
#include "AudioFile.h"
#include "AudioClip.h"
#include "ImageFile.h"
#include "Texture2D.h"
#include "IconManager.h"
#include "MeshAssetFile.h"
#include "MaterialAssetFile.h"
#include "AudioClipAssetFile.h"
#include "Texture2DAssetFile.h"

File::File()
{
}


File::File(const String &filepath)
#ifdef BANG_EDITOR
    : File(Explorer::GetInstance()->GetFileSystemModel(),
           Explorer::GetInstance()->GetModelIndexFromFilepath(filepath))
#endif
{
}

File::File(const QFileSystemModel *model, const QModelIndex &index) :
    m_fileSystemModel(model), m_modelIndex(index)
{
    m_isFile = !model->isDir(index);

    m_path = model->filePath(index).toStdString();
    m_path = IO::ToAbsolute(m_path, false); // To canonical
    m_name = IO::GetFileName(m_path);
    m_extension = IO::GetFileExtensionComplete(m_path);
}

File::~File()
{
}

bool File::IsAudioFile() const
{
    return m_isFile && IsOfExtension("wav ogg pcm");
}

bool File::IsAudioClipAsset() const
{
    return m_isFile && IsOfExtension(AudioClip::GetFileExtensionStatic());
}

bool File::IsTexture2DAsset() const
{
    return  m_isFile && IsOfExtension(Texture2D::GetFileExtensionStatic());
}

bool File::IsImageFile() const
{
    return  m_isFile && IsOfExtension("jpg jpeg png bmp tiff");
}

bool File::IsScene() const
{
    return m_isFile && IsOfExtension(Scene::GetFileExtensionStatic());
}

bool File::IsMeshAsset() const
{
    return  m_isFile && IsOfExtension(Mesh::GetFileExtensionStatic());
}

bool File::IsMeshFile() const
{
    return  m_isFile && IsOfExtension("obj");
}

bool File::IsMaterialAsset() const
{
    return m_isFile && IsOfExtension(Material::GetFileExtensionStatic());
}

bool File::IsBehaviour() const
{
    return m_isFile && IsOfExtension("cpp hpp c h");
}

bool File::IsTextFile() const
{
    return m_isFile && IsOfExtension("txt frag vert");
}

bool File::IsFontAssetFile() const
{
    return m_isFile && IsOfExtension(Font::GetFileExtensionStatic());
}

bool File::IsPrefabAsset() const
{
    return m_isFile && IsOfExtension(Prefab::GetFileExtensionStatic());
}

bool File::IsDir() const
{
    return !m_isFile;
}

bool File::IsFile() const
{
    return m_isFile;
}

bool File::IsOfExtension(const String &extensions) const
{
    // We receive something like "jpg png bmp obj"
    Array<String> extensionsList = extensions.Split(' ', true);
    for (String ext : extensionsList)
    {
        if (ext == GetExtension())
        {
            return true;
        }
    }
    return false;
}

File *File::GetSpecificFile(const File &f)
{
    if (!f.IsFile()) return nullptr;

    if (f.IsAudioClipAsset())
    {
        return new AudioClipAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsAudioFile())
    {
        return new AudioFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsTexture2DAsset())
    {
        return new Texture2DAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsImageFile())
    {
        return new ImageFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMaterialAsset())
    {
        return new MaterialAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMeshAsset())
    {
        return new MeshAssetFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsMeshFile())
    {
        return new MeshFile(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsPrefabAsset())
    {
        return new File(f.m_fileSystemModel, f.m_modelIndex);
    }
    else if (f.IsTextFile())
    {
        return new TextFile(f.m_fileSystemModel, f.m_modelIndex);
    }

    return new File(f.m_fileSystemModel, f.m_modelIndex);
}

bool File::Exists(const String &filepath)
{
    String absFilepath = IO::ToAbsolute(filepath, false);
    QFileInfo f(absFilepath.ToQString());
    return f.exists();
}

void File::Write(const String &filepath, const String &contents)
{
    std::ofstream out(filepath);
    if (out.is_open())
    {
        out << contents;
        out.close();
    }
}

void File::Write(const String &filepath, const Array<String> &lines)
{
    String all = String::Join(lines, "\n");
    File::Write(filepath, all);
}

void File::Write(const String &filepath, const List<String> &lines)
{
    String all = String::Join(lines, "\n");
    File::Write(filepath, all);
}

String File::GetContents(const String &filepath)
{
    return IO::GetFileContents(filepath);
}

String File::GetContents() const
{
    return IO::GetFileContents(m_path);
}

const QPixmap& File::GetIcon() const
{
    String fp = "";
    if (IsPrefabAsset())
    {
        fp = IO::ToAbsolute("./Icons/PrefabAssetIcon.png", true);
    }
    else if (IsBehaviour())
    {
        fp = IO::ToAbsolute("./Icons/BehaviourIcon.png", true);
    }
    else if (IsScene())
    {
        fp = IO::ToAbsolute("./Icons/SceneIcon.png", true);
    }
    else if (IsFontAssetFile())
    {
        fp = IO::ToAbsolute("./Icons/LetterIcon.png", true);
    }
    else
    {
        fp = IO::ToAbsolute("./Icons/OtherFileIcon.png", true);
    }

    // Its a texture, the icon is the image itself
    const QPixmap &pm =
         IconManager::LoadPixmap(fp, IconManager::IconOverlay::None, false);
    return pm;
}

#ifdef BANG_EDITOR
SerializableObject *File::GetInspectable() const
{
    return nullptr;
}
#endif

const String &File::GetAbsolutePath() const
{
    return m_path;
}

const String& File::GetName() const
{
    return m_name;
}

const String &File::GetRelativePath() const
{
    return m_path;
}

const String &File::GetExtension() const
{
    return m_extension;
}

String File::GetNameAndExtension() const
{
    String str = GetName();
    str += ".";
    str += GetExtension();
    return  str;
}

bool File::IsAsset() const
{
    return IsFontAssetFile() || IsTexture2DAsset() || IsMaterialAsset() ||
           IsMeshAsset() || IsAudioClipAsset() || IsPrefabAsset() ||
           IsBehaviour() || IsScene();
}

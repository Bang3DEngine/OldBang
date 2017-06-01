#include "Bang/File.h"

#include <fstream>
#include <QPainter>
#include "Bang/WinUndef.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#include "Bang/SerializableObject.h"
#endif

#include "Bang/Font.h"
#include "Bang/Mesh.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/TextFile.h"
#include "Bang/MeshFile.h"
#include "Bang/ModelFile.h"
#include "Bang/SoundFile.h"
#include "Bang/AudioClip.h"
#include "Bang/ImageFile.h"
#include "Bang/Texture2D.h"
#include "Bang/PrefabFile.h"
#include "Bang/IconManager.h"
#include "Bang/IInspectable.h"
#include "Bang/MaterialFile.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AudioClipFile.h"
#include "Bang/Texture2DFile.h"
#include "Bang/ShaderProgramFile.h"

File::File()
{
}


File::File(const Path &filepath)
{
    m_path = filepath;
}

File::File(const String &filepath) : File( Path(filepath) )
{
}

File::File(const QFileSystemModel *model, const QModelIndex &index)
{
    String pathStr(model->filePath(index).toStdString());
    m_path = Path(pathStr);
}

File::~File()
{
}

bool File::IsSound() const
{
    return GetPath().IsFile() && GetPath().HasExtension({"wav", "ogg", "pcm"});
}

bool File::IsAudioClipFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(AudioClip::GetFileExtensionStatic());
}

bool File::IsTexture2DFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Texture2D::GetFileExtensionStatic());
}

bool File::IsImageFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension({"jpg", "jpeg", "png", "bmp", "tiff", "tga"});
}

bool File::IsScene() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Scene::GetFileExtensionStatic());
}

bool File::IsMeshFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Mesh::GetFileExtensionStatic());
}

bool File::IsModelFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension({"obj", "mb", "fbx", "dae", "3ds", "ply",
                                    "stl", "ase", "blend", "md2", "md3"});
}

bool File::IsMaterialFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Material::GetFileExtensionStatic());
}

bool File::IsBehaviour() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension({"cpp", "hpp", "c", "h"});
}

bool File::IsTextFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension({"txt", "frag", "vert"});
}

bool File::IsFontFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Font::GetFileExtensionStatic());
}

bool File::IsPrefabFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Prefab::GetFileExtensionStatic());
}

bool File::IsShaderProgramFile() const
{
    return GetPath().IsFile() &&
            GetPath().HasExtension(ShaderProgram::GetFileExtensionStatic());
}

void File::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
}

bool File::DuplicateFile(const Path &fromFilepath,
                         const Path &toFilepath,
                         bool overwrite)
{
    if (!fromFilepath.IsFile()) { return false; }
    if (overwrite) { File::Remove(toFilepath); }
    bool ok = QFile::copy(fromFilepath.GetAbsolute().ToQString(),
                          toFilepath.GetAbsolute().ToQString());
    return ok;
}

bool File::DuplicateDir(const Path &fromDirpath,
                        const Path &toDirpath,
                        bool overwrite)
{
    if (!fromDirpath.IsDir()) { return false; }

    if (!File::CreateDirectory( Path(toDirpath) )) { return false; }
    List<Path> filepaths = fromDirpath.GetFiles(false);
    for(const Path& filepath : filepaths)
    {
        String fileName = filepath.GetNameExt();
        bool ok = File::DuplicateFile(fromDirpath.Append(fileName),
                                    toDirpath.Append(fileName),
                                    overwrite);
        if (!ok) { return false; }
    }

    List<Path> subdirs = fromDirpath.GetSubDirectories(false);
    for (const Path &subdir : subdirs)
    {
        bool ok = File::DuplicateDir(subdir,
                                   toDirpath.Append(subdir.GetName()),
                                   overwrite);
        if (!ok) { return false; }
    }
    return true;
}

bool File::Remove(const Path &path)
{
    if (!path.Exists()) { return false; }
    if (path.IsFile())
    {
        QFile f(path.GetAbsolute().ToQString());
        return f.remove();
    }
    else
    {
        List<Path> subDirs  = path.GetSubDirectories(false);
        for (const Path &subDir : subDirs) { File::Remove(subDir); }
        List<Path> subFiles = path.GetFiles(false);
        for (const Path &subFile : subFiles) { File::Remove(subFile); }
        QDir().rmdir(path.GetAbsolute().ToQString());
    }
    return false;
}

bool File::CreateDirectory(const Path &dirPath)
{
    if (dirPath.IsEmpty()) { return false; }
    if (dirPath.IsDir()) { return true; }
    return QDir().mkdir(dirPath.GetAbsolute().ToQString());
}

bool File::Move(const Path &oldPath, const Path &newPath)
{
    if (!oldPath.Exists()) { return false; }
    if (oldPath.IsDir())
    {
        return QDir().rename(oldPath.GetAbsolute().ToQString(),
                             newPath.GetAbsolute().ToQString());
    }
    return QFile().rename(oldPath.GetAbsolute().ToQString(),
                          newPath.GetAbsolute().ToQString());
}

bool File::Duplicate(const Path &fromPath, const Path &toPath)
{
    if (fromPath.IsFile())
    {
        return File::DuplicateFile(fromPath, toPath);
    }
    else if (fromPath.IsDir())
    {
        return File::DuplicateDir(fromPath, toPath);
    }
    return false;
}

File *File::GetSpecificFile(const File &f)
{
    if (!f.GetPath().IsFile()) { return nullptr; }

    if (f.IsAudioClipFile())
    {
        return new AudioClipFile( f.GetPath() );
    }
    else if (f.IsSound())
    {
        return new SoundFile( f.GetPath() );
    }
    else if (f.IsTexture2DFile())
    {
        return new Texture2DFile( f.GetPath() );
    }
    else if (f.IsImageFile())
    {
        return new ImageFile( f.GetPath() );
    }
    else if (f.IsMaterialFile())
    {
        return new MaterialFile( f.GetPath() );
    }
    else if (f.IsMeshFile())
    {
        return new MeshFile( f.GetPath() );
    }
    else if (f.IsModelFile())
    {
        return new ModelFile( f.GetPath() );
    }
    else if (f.IsPrefabFile())
    {
        return new PrefabFile( f.GetPath() );
    }
    else if (f.IsTextFile())
    {
        return new TextFile( f.GetPath() );
    }
    else if (f.IsShaderProgramFile())
    {
        return new ShaderProgramFile( f.GetPath() );
    }

    return new File( f.GetPath() );
}

void File::Write(const Path &filepath, const String &contents)
{
    std::ofstream out(filepath.GetAbsolute());
    if (out)
    {
        out << contents;
        out.close();
    }
}

void File::Write(const Path &filepath, const Array<String> &lines)
{
    File::Write(filepath, String::Join(lines, "\n"));
}

void File::Write(const Path &filepath, const List<String> &lines)
{
    File::Write(filepath, String::Join(lines, "\n"));
}

String File::GetContents(const Path &filepath)
{
    if (!filepath.IsFile()) { return ""; }

    String contents = "";
    std::ifstream ifs(filepath.GetAbsolute().ToCString());
    if (ifs)
    {
        contents = String((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
        ifs.close();
    }
    else
    {
        Debug_Error("Can't open file '" << filepath << "': " <<
                    std::strerror(errno));
    }
    return contents;
}

String File::GetHash(const Path &filepath)
{
    if (!filepath.IsFile()) { return ""; }

    String result = "";
    QFile file(filepath.GetAbsolute().ToQString());
    if(file.open(QIODevice::ReadOnly))
    {
        result = File::GetHashFromByteArray(file.readAll());
        file.close();
    }
    return result;
}

String File::GetHashFromString(const String &str)
{
    return File::GetHashFromByteArray( str.ToQString().toUtf8() );
}

String File::GetHashFromByteArray(const QByteArray &byteArray)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QByteArray hashBytes = hash.hash(byteArray, QCryptographicHash::Sha1);
    return String( QString(hashBytes.toHex()) );
}

String File::GetContents() const
{
    return File::GetContents( GetPath() );
}

const Path &File::GetPath() const
{
    return m_path;
}

const QPixmap& File::GetIcon() const
{
    Path iconPath;
    if (IsPrefabFile())
    {
        iconPath = EPATH("Icons/PrefabAssetIcon.png");
    }
    else if (IsBehaviour())
    {
        iconPath = EPATH("Icons/BehaviourIcon.png");
    }
    else if (IsScene())
    {
        iconPath = EPATH("Icons/SceneIcon.png");
    }
    else if (IsFontFile())
    {
        iconPath = EPATH("Icons/LetterIcon.png");
    }
    else
    {
        iconPath = EPATH("Icons/OtherFileIcon.png");
    }

    // Its a texture, the icon is the image itself
    const QPixmap &pm =
         IconManager::LoadPixmap(iconPath, IconManager::IconOverlay::None);
    return pm;
}

void File::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
}

#ifdef BANG_EDITOR
IInspectable* File::GetNewInspectable()
{
    return nullptr;
}
#endif

bool File::IsAsset() const
{
    return IsFontFile() || IsTexture2DFile() || IsMaterialFile() ||
           IsMeshFile() || IsAudioClipFile() || IsPrefabFile() ||
           IsBehaviour() || IsScene();
}

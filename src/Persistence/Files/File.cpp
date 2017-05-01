#include "Bang/File.h"

#include <fstream>
#include <QPainter>
#include "Bang/WinUndef.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#include "Bang/SerializableObject.h"
#endif

#include "Bang/IO.h"
#include "Bang/Font.h"
#include "Bang/Mesh.h"
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
#include "Bang/MeshAssetFile.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialAssetFile.h"
#include "Bang/AudioClipAssetFile.h"
#include "Bang/Texture2DAssetFile.h"
#include "Bang/ShaderProgramAssetFile.h"

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
    m_path = GPATH(pathStr);
}

bool File::IsSound() const
{
    return GetPath().IsFile() && GetPath().HasExtension({"wav", "ogg", "pcm"});
}

bool File::IsAudioClipAsset() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(AudioClip::GetFileExtensionStatic());
}

bool File::IsTexture2DAsset() const
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

bool File::IsMaterialAsset() const
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

bool File::IsFontAssetFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Font::GetFileExtensionStatic());
}

bool File::IsPrefabAsset() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Prefab::GetFileExtensionStatic());
}

bool File::IsShaderProgramAssetFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(ShaderProgram::GetFileExtensionStatic());
}

File *File::GetSpecificFile(const File &f)
{
    if (!f.GetPath().IsFile()) { return nullptr; }

    if (f.IsAudioClipAsset())
    {
        return new AudioClipAssetFile( f.GetPath() );
    }
    else if (f.IsSound())
    {
        return new SoundFile( f.GetPath() );
    }
    else if (f.IsTexture2DAsset())
    {
        return new Texture2DAssetFile( f.GetPath() );
    }
    else if (f.IsImageFile())
    {
        return new ImageFile( f.GetPath() );
    }
    else if (f.IsMaterialAsset())
    {
        return new MaterialAssetFile( f.GetPath() );
    }
    else if (f.IsMeshFile())
    {
        return new MeshFile( f.GetPath() );
    }
    else if (f.IsModelFile())
    {
        return new ModelFile( f.GetPath() );
    }
    else if (f.IsPrefabAsset())
    {
        return new PrefabFile( f.GetPath() );
    }
    else if (f.IsTextFile())
    {
        return new TextFile( f.GetPath() );
    }
    else if (f.IsShaderProgramAssetFile())
    {
        return new ShaderProgramAssetFile( f.GetPath() );
    }

    return new File( f.GetPath() );
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
    if (out)
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
    return IO::GetFileContents( Path(filepath) );
}

String File::GetContents() const
{
    return IO::GetFileContents( GetPath() );
}

const Path &File::GetPath() const
{
    return m_path;
}

const QPixmap& File::GetIcon() const
{
    Path iconPath;
    if (IsPrefabAsset())
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
    else if (IsFontAssetFile())
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
    xmlInfo->SetTagName( GetPath().GetName() );
}

#ifdef BANG_EDITOR
IInspectable* File::GetNewInspectable()
{
    return nullptr;
}
#endif

bool File::IsAsset() const
{
    return IsFontAssetFile() || IsTexture2DAsset() || IsMaterialAsset() ||
           IsMeshFile() || IsAudioClipAsset() || IsPrefabAsset() ||
           IsBehaviour() || IsScene();
}

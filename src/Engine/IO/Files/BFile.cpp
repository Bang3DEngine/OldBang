#include "Bang/BFile.h"

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
#include "Bang/MaterialFile.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AudioClipFile.h"
#include "Bang/Texture2DFile.h"
#include "Bang/ShaderProgramFile.h"

BFile::BFile() : File()
{
}


BFile::BFile(const Path &filepath) : File(filepath)
{
}

BFile::BFile(const String &filepath) : File(filepath)
{
}

BFile::BFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
}

BFile::~BFile()
{
}

bool BFile::IsSound() const
{
    return GetPath().IsFile() && GetPath().HasExtension({"wav", "ogg", "pcm"});
}

bool BFile::IsAudioClipFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(AudioClip::GetFileExtensionStatic());
}

bool BFile::IsTexture2DFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Texture2D::GetFileExtensionStatic());
}

bool BFile::IsImageFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension({"jpg", "jpeg", "png", "bmp", "tiff", "tga"});
}

bool BFile::IsScene() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Scene::GetFileExtensionStatic());
}

bool BFile::IsMeshFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Mesh::GetFileExtensionStatic());
}

bool BFile::IsModelFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension({"obj", "mb", "fbx", "dae", "3ds", "ply",
                                    "stl", "ase", "blend", "md2", "md3"});
}

bool BFile::IsMaterialFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Material::GetFileExtensionStatic());
}

bool BFile::IsBehaviour() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension({"cpp", "hpp", "c", "h"});
}

bool BFile::IsTextFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension({"txt", "frag", "vert"});
}

bool BFile::IsFontFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Font::GetFileExtensionStatic());
}

bool BFile::IsPrefabFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Prefab::GetFileExtensionStatic());
}

bool BFile::IsShaderProgramFile() const
{
    return GetPath().IsFile() &&
            GetPath().HasExtension(ShaderProgram::GetFileExtensionStatic());
}

void BFile::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
}

BFile *BFile::GetSpecificFile(const BFile &f)
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

    return new BFile( f.GetPath() );
}

const QPixmap& BFile::GetIcon() const
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

void BFile::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
}

bool BFile::IsAsset() const
{
    return IsFontFile() || IsTexture2DFile() || IsMaterialFile() ||
           IsMeshFile() || IsAudioClipFile() || IsPrefabFile() ||
           IsBehaviour() || IsScene();
}

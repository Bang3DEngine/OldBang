#include "Bang/BFile.h"

#include <fstream>
#include <QPainter>

#include "Bang/Font.h"
#include "Bang/Mesh.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/TextFile.h"
#include "Bang/MeshFile.h"
#include "Bang/Behaviour.h"
#include "Bang/ModelFile.h"
#include "Bang/SoundFile.h"
#include "Bang/AudioClip.h"
#include "Bang/ImageFile.h"
#include "Bang/Texture2D.h"
#include "Bang/Extensions.h"
#include "Bang/PrefabFile.h"
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
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::GetList<SoundFile>());
}

bool BFile::IsAudioClipFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::Get<AudioClip>());
}

bool BFile::IsTexture2DFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Extensions::Get<Texture2D>());
}

bool BFile::IsImageFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Extensions::GetList<ImageFile>());
}

bool BFile::IsScene() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::Get<Scene>());
}

bool BFile::IsMeshFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Extensions::Get<Mesh>());
}

bool BFile::IsModelFile() const
{
    return  GetPath().IsFile() &&
            GetPath().HasExtension(Extensions::GetList<ModelFile>());
}

bool BFile::IsMaterialFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::Get<Material>());
}

bool BFile::IsBehaviour() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::GetList<Behaviour>());
}

bool BFile::IsTextFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::GetTextFileList());
}

bool BFile::IsFontFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::Get<Font>());
}

bool BFile::IsPrefabFile() const
{
    return GetPath().IsFile() &&
           GetPath().HasExtension(Extensions::Get<Prefab>());
}

bool BFile::IsShaderProgramFile() const
{
    return GetPath().IsFile() &&
            GetPath().HasExtension(Extensions::Get<ShaderProgram>());
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

#include "Bang/Extensions.h"

#include "Bang/Mesh.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/Project.h"
#include "Bang/Material.h"
#include "Bang/ModelFile.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/ImageFile.h"
#include "Bang/SoundFile.h"

template<> String Extensions::Get<Mesh>() { return "bmesh"; }
template<> String Extensions::Get<Font>() { return "bfont"; }
template<> String Extensions::Get<Prefab>() { return "bprefab"; }
template<> String Extensions::Get<Material>() { return "bmat"; }
template<> String Extensions::Get<AudioClip>() { return "baudioclip"; }
template<> String Extensions::Get<Texture2D>() { return "btex2d"; }
template<> String Extensions::Get<ShaderProgram>() { return "bshaderprogram"; }

template<> String Extensions::Get<Scene>() { return "bscene"; }
template<> String Extensions::Get<Project>() { return "bproject"; }

template<> List<String> Extensions::GetList<Behaviour>()
{
    return {"c", "cpp", "h", "hpp"};
}
template<> List<String> Extensions::GetList<SoundFile>()
{
    return {"wav", "ogg", "pcm"};
}
template<> List<String> Extensions::GetList<ModelFile>()
{
    return {"obj", "mb", "fbx", "dae", "3ds", "ply",
            "stl", "ase", "blend", "md2", "md3"};
}
template<> List<String> Extensions::GetList<ImageFile>()
{
    return {"jpg", "jpeg", "png", "bmp", "tiff", "tga"};
}

List<String> Extensions::GetTextFileList()
{
    return {"txt", "frag", "vert", "glsl"}
}

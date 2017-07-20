#include "Bang/Extensions.h"

#include "Bang/Mesh.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/Project.h"
#include "Bang/Material.h"
#include "Bang/Behaviour.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"

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

List<String> Extensions::GetTTFList()
{
    return {"ttf"};
}

List<String> Extensions::GetTextFileList()
{
    return {"txt"};
}

List<String> Extensions::GetSourceFileList()
{
    return {"c", "cpp"};
}

List<String> Extensions::GetVertexShaderList()
{
    return {"vert", "vert_*"};
}

List<String> Extensions::GetFragmentShaderList()
{
    return {"frag", "frag_*"};
}

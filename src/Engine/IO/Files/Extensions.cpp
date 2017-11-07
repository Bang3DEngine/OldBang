#include "Bang/Extensions.h"

#include "Bang/List.h"
#include "Bang/Mesh.h"
#include "Bang/Font.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/Prefab.h"
#include "Bang/Project.h"
#include "Bang/Material.h"
#include "Bang/Behaviour.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

String Extensions::GetFontExtension() { return "bfont"; }
String Extensions::GetSceneExtension() { return "bscene"; }
String Extensions::GetPrefabExtension() { return "bprefab"; }
String Extensions::GetProjectExtension() { return "bproject"; }
String Extensions::GetMaterialExtension() { return "bmat"; }
String Extensions::GetAudioClipExtension() { return "baudioclip"; }
String Extensions::GetTexture2DExtension() { return "btex2d"; }
String Extensions::GetShaderProgramExtension() { return "bshaderprogram"; }

List<String> Extensions::GetTTFExtensions()
{
    return {"ttf"};
}

List<String> Extensions::GetTextFileExtensions()
{
    return {"txt"};
}

List<String> Extensions::GetBehaviourExtensions()
{
    return {"c", "cpp", "h", "hpp"};
}

List<String> Extensions::GetSourceFileExtensions()
{
    return {"c", "cpp"};
}

List<String> Extensions::GetVertexShaderExtensions()
{
    return {"vert", "vert_*"};
}

List<String> Extensions::GetFragmentShaderExtensions()
{
    return {"frag", "frag_*"};
}

NAMESPACE_BANG_END

#include "Bang/Extensions.h"

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

String Extensions::GetSceneExtension() { return "bscene"; }
String Extensions::GetPrefabExtension() { return "bprefab"; }
String Extensions::GetProjectExtension() { return "bproject"; }
String Extensions::GetMaterialExtension() { return "bmat"; }
String Extensions::GetShaderProgramExtension() { return "bshaderprogram"; }

Array<String> Extensions::GetTTFExtensions()
{
    return {"ttf"};
}

Array<String> Extensions::GetImageExtensions()
{
    return {"png", "jpg", "jpeg", "bmp", "tiff"};
}

Array<String> Extensions::GetModelExtensions()
{
    return {"obj", "ply"};
}

Array<String> Extensions::GetAudioClipExtensions()
{
    return {"ogg", "wav"};
}

Array<String> Extensions::GetTextFileExtensions()
{
    return {"txt"};
}

Array<String> Extensions::GetBehaviourExtensions()
{
    return {"c", "cpp", "h", "hpp"};
}

Array<String> Extensions::GetSourceFileExtensions()
{
    return {"c", "cpp"};
}

Array<String> Extensions::GetVertexShaderExtensions()
{
    return {"vert", "vert_*"};
}

Array<String> Extensions::GetFragmentShaderExtensions()
{
    return {"frag", "frag_*"};
}

String Extensions::GetDefaultBehaviourHeaderExtension()
{
    return "h";
}

String Extensions::GetDefaultBehaviourSourceExtension()
{
    return "cpp";
}

bool Extensions::Has(const Path &path, const Array<String> &extensions)
{
    return Extensions::Equals(path.GetExtension(), extensions);
}

bool Extensions::Equals(const String &extension, const Array<String> &extensions)
{
    return extensions.Contains(extension.ToLower());
}

bool Extensions::Equals(const String &extensionSrc, const String &extensionDst)
{
    return extensionSrc.ToLower() == extensionDst.ToLower();
}

NAMESPACE_BANG_END

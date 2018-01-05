#include "Bang/Extensions.h"

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

String Extensions::GetFontExtension() { return "bfont"; }
String Extensions::GetSceneExtension() { return "bscene"; }
String Extensions::GetPrefabExtension() { return "bprefab"; }
String Extensions::GetProjectExtension() { return "bproject"; }
String Extensions::GetMaterialExtension() { return "bmat"; }
String Extensions::GetAudioClipExtension() { return "baudioclip"; }
String Extensions::GetTexture2DExtension() { return "btex2d"; }
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

NAMESPACE_BANG_END

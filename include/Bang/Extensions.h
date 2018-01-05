#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Extensions
{
public:
    static String GetFontExtension();
    static String GetSceneExtension();
    static String GetPrefabExtension();
    static String GetProjectExtension();
    static String GetMaterialExtension();
    static String GetAudioClipExtension();
    static String GetTexture2DExtension();
    static String GetShaderProgramExtension();

    static Array<String> GetTTFExtensions();
    static Array<String> GetImageExtensions();
    static Array<String> GetModelExtensions();
    static Array<String> GetTextFileExtensions();
    static Array<String> GetBehaviourExtensions();
    static Array<String> GetSourceFileExtensions();
    static Array<String> GetVertexShaderExtensions();
    static Array<String> GetFragmentShaderExtensions();

    static String GetDefaultBehaviourHeaderExtension();
    static String GetDefaultBehaviourSourceExtension();

    static bool Has(const Path &path, const Array<String> &extensions);
    static bool Equals(const String &extension, const Array<String> &extensions);

    Extensions() = delete;
};

NAMESPACE_BANG_END

#endif // EXTENSIONS_H

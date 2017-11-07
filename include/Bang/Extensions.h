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

    static List<String> GetTTFExtensions();
    static List<String> GetTextFileExtensions();
    static List<String> GetBehaviourExtensions();
    static List<String> GetSourceFileExtensions();
    static List<String> GetVertexShaderExtensions();
    static List<String> GetFragmentShaderExtensions();

    Extensions() = delete;
};

NAMESPACE_BANG_END

#endif // EXTENSIONS_H

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Extensions
{
public:
    template<class T> static String Get();
    template<class T> static List<String> GetList();

    static List<String> GetTTFList();
    static List<String> GetTextFileList();
    static List<String> GetSourceFileList();
    static List<String> GetVertexShaderList();
    static List<String> GetFragmentShaderList();

    Extensions() = delete;
};

NAMESPACE_BANG_END

#endif // EXTENSIONS_H

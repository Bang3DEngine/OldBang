#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "Bang.h"

FORWARD_T class List;
FORWARD   class String;

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

private:
    Extensions() = delete;
};

#endif // EXTENSIONS_H

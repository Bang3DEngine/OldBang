#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "Bang/List.h"
#include "Bang/String.h"

class Extensions
{
public:
    template<class T> static String Get();
    template<class T> static List<String> GetList();
    static List<String> GetTextFileList();

private:
    Extensions() = delete;
};

#endif // EXTENSIONS_H

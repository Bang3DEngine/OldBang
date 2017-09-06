#ifndef LIBRARY_H
#define LIBRARY_H

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

class Library
{
public:
    Library(const Path& libPath);
    ~Library();


    template<class T>
    T Get(const String &symbolName)
    {
        return RCAST<T>( GetSymbol(symbolName) );
    }

    bool Load();
    bool UnLoad();
    bool IsLoaded() const;
    void* GetSymbol(const String &symbolName);

    const Path& GetLibraryPath() const;
    const String& GetErrorString() const;

private:
    Path m_libPath;
    void *m_libHandle = nullptr;
    String m_errorString;

    void ClearError();
    void FetchError();
    bool TheresError() const;
};

NAMESPACE_BANG_END

#endif // LIBRARY_H

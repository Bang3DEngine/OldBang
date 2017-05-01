#ifndef PATH_H
#define PATH_H

#include <QFileInfo>

#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

// User assets path
#define UPATH(path) Path(path, IO::GetProjectAssetsRootAbs())

// Engine assets path
#define EPATH(path) Path(path, IO::GetEngineAssetsRootAbs())

// Generic path, to use when passing an absolute file, it detects whether
// it is a user or engine path
#define GPATH(absPath) \
    (absPath.BeginsWith( IO::GetEngineAssetsRootAbs() ) ?  \
        EPATH(absPath) : UPATH(absPath) )

class Path : public IToString
{
public:
    Path();
    Path(const Path &path);
    explicit Path(const String &absolutePath);
    Path(const String &relativePath,
         const String &absolutePrefix);

    void SetPath(const String &relativePath,
                 const String &absolutePrefix = "");

    bool IsDir() const;
    bool IsFile() const;
    bool Exists() const;

    List<Path> GetFiles(bool recursively = false,
                        const List<String>& extensions = {}) const;
    List<Path> GetSubDirectories(bool recursively = false) const;

    Path GetDirectory() const;
    String GetName() const;
    String GetNameExt() const;
    String GetExtension() const;
    const String& GetAbsolute() const;
    const String& GetRelative() const;

    virtual String ToString() const override;
    bool IsEmpty() const;

    Path Append(const String& str) const;
    Path AppendExtension(const String& extension) const;

    // "*.jpg *.so.*"
    bool HasExtension(const String &extension) const;
    bool HasExtension(const List<String> &extensions) const;

    bool operator!=(const Path &rhs) const;
    bool operator==(const Path &rhs) const;
    bool operator<(const Path &rhs) const;

    static const Path Empty;

private:
    String m_relativePath   = "";
    String m_absolutePath   = "";
    String m_absolutePrefix = "";

};

#endif // PATH_H

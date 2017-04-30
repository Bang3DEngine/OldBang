#ifndef PATH_H
#define PATH_H

#include <QFileInfo>

#include "Bang/String.h"
#include "Bang/IToString.h"

// User assets path
#define UPATH(path) Path(path, IO::GetProjectAssetsRootAbs())

// Engine assets path
#define EPATH(path) Path(path, IO::GetEngineAssetsRootAbs())

class Path : public IToString
{
public:
    Path();
    Path(const String &absolutePath);
    Path(const String &relativePath,
         const String &absolutePrefix);

    void SetPath(const String &relativePath,
                 const String &absolutePrefix = "");

    bool IsDir() const;
    bool IsFile() const;
    bool Exists() const;

    List<Path> GetFiles(bool recursively = false);
    List<Path> GetSubDirectories(bool recursively = false);

    Path GetParentDirectory() const;
    String GetBaseName() const;
    String GetBaseNameExt() const;
    String GetExtension() const;
    const String& GetAbsolute() const;
    const String& GetRelative() const;

    String ToString() const;

private:
    String m_relativePath   = "";
    String m_absolutePath   = "";
    String m_absolutePrefix = "";

};

#endif // PATH_H

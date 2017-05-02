#include "Path.h"

#include "Bang/IO.h"
#include "Bang/Array.h"
#include <iostream>

const Path Path::Empty;

Path::Path()
{
}

Path::Path(const Path &path) : Path(path.GetAbsolute())
{
}

Path::Path(const String &absolutePath)
{
    SetPath(absolutePath);
}

void Path::SetPath(const String &path)
{
    m_absolutePath = path;
    if (!m_absolutePath.Empty() &&
         m_absolutePath.At(m_absolutePath.Length()-1) == '/')
    {
        m_absolutePath.Erase(m_absolutePath.Length()-1,
                             m_absolutePath.Length()-1);
    }

    if (m_absolutePath.BeginsWith("./"))
    {
        m_absolutePath.Erase(0, 1);
    }
}

bool Path::IsDir() const
{
    return IO::IsDir( GetAbsolute() );
}

bool Path::IsFile() const
{
    return IO::IsFile( GetAbsolute() );
}

bool Path::Exists() const
{
    return IO::Exists( GetAbsolute() );
}

List<Path> Path::GetFiles(bool recursive, const List<String> &extensions) const
{
    List<Path> filePaths;
    List<String> filesList = IO::GetFiles(GetAbsolute(), recursive, extensions);
    for (const String &file : filesList)
    {
        filePaths.Add( Path(file) );
    }
    return filePaths;
}

List<Path> Path::GetSubDirectories(bool recursively) const
{
    List<Path> dirPaths;
    List<String> dirsList = IO::GetSubDirectories(GetAbsolute(), recursively);
    for (String &dir : dirsList)
    {
        dirPaths.Add( Path(dir) );
    }
    return dirPaths;
}

String Path::GetName() const
{
    return IO::GetBaseName( GetAbsolute() );
}

String Path::GetNameExt() const
{
    String ext = GetExtension();
    if (ext.Empty()) { return GetName(); }
    return GetName() + "." + ext;
}

String Path::GetExtension() const
{
    return IO::GetFileExtensionComplete( GetAbsolute() );
}

Path Path::GetDirectory() const
{
    String dirPath = IO::GetDir( GetAbsolute() );
    return Path(dirPath);
}

const String &Path::GetAbsolute() const
{
    return m_absolutePath;
}

String Path::GetRelative() const
{
    const String &engineAssets  = IO::GetEngineAssetsRootAbs();
    const String &projectAssets = IO::GetProjectAssetsRootAbs();
    const String &projectRoot   = IO::GetProjectRootAbs();
    const String &engineRoot    = IO::GetEngineRootAbs();
    if (GetAbsolute().BeginsWith(engineAssets))
    {
        return GetAbsolute().SubString(engineAssets.Length() + 1);
    }
    else if (GetAbsolute().BeginsWith(projectAssets))
    {
        return GetAbsolute().SubString(projectAssets.Length() + 1);
    }
    else if (GetAbsolute().BeginsWith(engineRoot))
    {
        return GetAbsolute().SubString(engineRoot.Length() + 1);
    }
    else if (GetAbsolute().BeginsWith(projectRoot))
    {
        return GetAbsolute().SubString(projectRoot.Length() + 1);
    }

    return GetAbsolute();
}

Path Path::GetDuplicate() const
{
    return Path(IO::GetDuplicatePath( GetAbsolute() ));
}

String Path::ToString() const
{
    return GetAbsolute();
}

bool Path::IsEmpty() const
{
    return GetAbsolute().Empty();
}

Path Path::Append(const Path &pathRHS) const
{
    String str = pathRHS.GetAbsolute();
    if (str.BeginsWith("./")) { str.Erase(0, 0); }
    while (str.BeginsWith("/")) { str.Erase(0, 0); }
    return this->AppendRaw("/" + str);
}

Path Path::Append(const String &str) const
{
    return Path(GetAbsolute()).Append( Path(str) );
}

Path Path::AppendRaw(const String &str) const
{
    return Path(GetAbsolute() + str);
}

Path Path::AppendExtension(const String &extension) const
{
    if (HasExtension(extension)) { return Path(*this); }
    return Path(GetAbsolute() + "." + extension);
}

bool Path::HasExtension(const String &extensions) const
{
    String extension = GetExtension();
    Array<String> extensionsList = extensions.Split(' ', true);
    for (const String& ext : extensionsList)
    {
        if ( ext.EqualsNoCase(extension) ) { return true; }
    }
    return false;
}

bool Path::HasExtension(const List<String> &extensions) const
{
    String extension = GetExtension();
    for (const String& ext : extensions)
    {
        if ( ext.EqualsNoCase(extension) ) { return true; }
    }
    return false;
}

Path::operator String() const
{
    return GetAbsolute();
}

bool Path::operator!=(const Path &rhs) const
{
    return !(*this == rhs);
}

bool Path::operator==(const Path &rhs) const
{
    return GetAbsolute() == rhs.GetAbsolute();
}

bool Path::operator<(const Path &rhs) const
{
    return GetAbsolute() < rhs.GetAbsolute();
}

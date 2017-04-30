#include "Path.h"

#include "Bang/IO.h"
#include "Bang/Array.h"

Path::Path()
{
}

Path::Path(const String &absolutePath)
{
    SetPath(absolutePath, "");
}

Path::Path(const String &relativePath, const String &absolutePrefix)
{
    SetPath(relativePath, absolutePrefix);
}

void Path::SetPath(const String &relativePath, const String &absolutePrefix)
{
    m_absolutePrefix = absolutePrefix;
    if (!m_absolutePrefix.Empty() &&
         m_absolutePrefix.At(m_absolutePrefix.Length()-1) != '/')
    {
        m_absolutePrefix += "/";
    }

    m_relativePath = IO::ToRelative( relativePath, m_absolutePrefix );
    m_absolutePath = m_absolutePrefix + m_relativePath;
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


List<Path> Path::GetFiles(bool recursively, const List<String> &extensions)
{
    List<Path> filePaths;
    List<String> filesList = IO::GetFiles(GetAbsolute(), recursively);
    for (const String &file : filesList)
    {
        Path filepath(file, m_absolutePrefix);
        if (extensions.Empty() || filepath.HasExtension(extensions))
        {
            filePaths.Add( Path(file, m_absolutePrefix) );
        }
    }
    return filePaths;
}

List<Path> Path::GetSubDirectories(bool recursively)
{
    List<Path> dirPaths;
    List<String> dirsList = IO::GetSubDirectories(GetAbsolute(), recursively);
    for (String &dir : dirsList)
    {
        dirPaths.Add( Path(dir, m_absolutePrefix) );
    }
    return dirPaths;
}

String Path::GetName() const
{
    return IO::GetBaseName( GetRelative() );
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
    return Path(dirPath, m_absolutePrefix);
}

const String &Path::GetAbsolute() const
{
    return m_absolutePath;
}

const String& Path::GetRelative() const
{
    return m_relativePath;
}

String Path::ToString() const
{
    return GetAbsolute();
}

bool Path::Empty() const
{
    return GetAbsolute().Empty();
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

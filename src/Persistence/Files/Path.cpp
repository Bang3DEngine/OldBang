#include "Path.h"

#include "Bang/IO.h"

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

List<Path> Path::GetFiles(bool recursively)
{
    List<Path> filePaths;
    List<String> filesList = IO::GetFiles(GetAbsolute(), recursively);
    for (String &file : filesList)
    {
        filePaths.Add( Path(file, m_absolutePrefix) );
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

String Path::GetBaseName() const
{
    return IO::GetBaseName( GetRelative() );
}

String Path::GetBaseNameExt() const
{
    String ext = GetExtension();
    if (ext.Empty()) { return GetBaseName(); }
    return GetBaseName() + "." + ext;
}

String Path::GetExtension() const
{
    return IO::GetFileExtensionComplete( GetAbsolute() );
}

Path Path::GetParentDirectory() const
{
    return IO::GetDir( GetAbsolute() );
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

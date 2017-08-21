#include "Path.h"

#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Bang/List.h"
#include "Bang/Array.h"

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
    if (!m_absolutePath.IsEmpty() &&
         m_absolutePath.At(m_absolutePath.Size()-1) == '/')
    {
        m_absolutePath.Erase(m_absolutePath.Size()-1,
                             m_absolutePath.Size()-1);
    }

    if (m_absolutePath.BeginsWith("./"))
    {
        m_absolutePath.Erase(0, 1);
    }
}

bool Path::IsDir() const
{
    if (!Exists()) { return false; }
    return !IsFile();
}

bool Path::IsFile() const
{
    if (!Exists()) { return false; }

    struct stat path_stat;
    stat(GetAbsolute().ToCString(), &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool Path::Exists() const
{
    return access(GetAbsolute().ToCString(), F_OK) != -1;
}

List<Path> Path::FindFiles(bool recursively) const
{
    return FindFiles(recursively, {});
}

List<Path> Path::FindFiles(bool recursive, const List<String> &extensions) const
{
    if (!IsDir()) { return {}; }

    if (!recursive)
    {
        List<Path> subFilesList;
        List<Path> subPathsList = FindSubPaths();
        for (const Path &subPath : subPathsList)
        {
            if ( subPath.IsFile() &&
                (extensions.IsEmpty() || subPath.HasExtension(extensions)) )
            {
                subFilesList.PushBack(subPath);
            }
        }
        return subFilesList;
    }
    else
    {
        List<Path> filesList;
        filesList.PushBack( FindFiles(false, extensions) );

        List<Path> subDirs = FindSubDirectories(true);
        for (const Path &subdir : subDirs)
        {
            List<Path> subFilesList = subdir.FindFiles(true, extensions);
            filesList.PushBack(subFilesList);
        }
        return filesList;
    }
}

List<Path> Path::FindSubDirectories(bool recursive) const
{
    if (!IsDir()) { return {}; }

    if (!recursive)
    {
        List<Path> subDirsList;
        List<Path> subPathsList = FindSubPaths();
        for (const Path &subPath : subPathsList)
        {
            if (subPath.IsDir()) { subDirsList.PushBack(subPath); }
        }
        return subDirsList;
    }
    else
    {
        List<Path> subdirsList = FindSubDirectories(false);
        for (Path subdirPath : subdirsList)
        {
            if (recursive)
            {
                List<Path> subdirsListRecursive =
                        subdirPath.FindSubDirectories(true);
                subdirsList.Splice(subdirsList.End(), subdirsListRecursive);
            }
        }
        return subdirsList;
    }
}

List<Path> Path::FindSubPaths() const
{
    struct dirent *dir;
    DIR *d = opendir(GetAbsolute().ToCString());
    if (!d) { return {}; }

    List<Path> subPathsList;
    while ((dir = readdir(d)) != NULL)
    {
        String subName(dir->d_name);
        if (!subName.BeginsWith("."))
        {
            Path subPath = this->Append(subName);
            subPathsList.PushBack(subPath);
        }
    }
    closedir(d);

    return subPathsList;
}

String Path::GetName() const
{
    Array<String> parts = GetNameExt().Split<Array>('.');
    return parts.Size() >= 1 ? parts[0] : "";
}

String Path::GetNameExt() const
{
    if (IsEmpty()) { return ""; }

    String filename = GetAbsolute();
    const size_t lastSlash = GetAbsolute().RFind('/');
    if (lastSlash != String::npos)
    {
        filename = GetAbsolute().SubString(lastSlash + 1);
    }
    return filename;
}

String Path::GetExtension() const
{
    Array<String> parts = GetNameExt().Split<Array>('.');
    return parts.Size() >= 2 ? parts[1] : "";
}

Path Path::GetDirectory() const
{
    if (IsEmpty()) { return Path::Empty; }

    const size_t lastSlash = GetAbsolute().RFind('/');
    if (lastSlash != String::npos)
    {
        return Path(GetAbsolute().SubString(0, lastSlash-1));
    }
    return Path(".");
}

const String &Path::GetAbsolute() const
{
    return m_absolutePath;
}

Path Path::GetDuplicate() const
{
    if (IsEmpty()) { return Path::Empty; }

    Path resultPath(*this);
    while (resultPath.Exists())
    {
        resultPath = Path::GetNextDuplicatePath(resultPath);
    }
    return resultPath;
}

String Path::ToString() const
{
    return GetAbsolute();
}

bool Path::IsEmpty() const
{
    return GetAbsolute().IsEmpty();
}

bool Path::BeginsWith(const Path &path) const
{
    return BeginsWith(path.GetAbsolute());
}

bool Path::BeginsWith(const String &path) const
{
    return GetAbsolute().BeginsWith(path);
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

Path Path::ChangeExtension(const String &extension) const
{
    return Path( GetDirectory().Append(GetName())
                               .AppendExtension(extension) );
}

bool Path::HasExtension(const String &extensions) const
{
    List<String> extensionsList = extensions.Split<List>(' ', true);
    return HasExtension(extensionsList);
}

bool Path::HasExtension(const List<String> &extensions) const
{
    String ext = GetExtension();
    for (const String &extension : extensions)
    {
        if (extension.EqualsNoCase(ext)) { return true; }
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

Path Path::GetNextDuplicatePath(const Path &filepath)
{
    if (filepath.IsEmpty()) { return Path::Empty; }

    Path fileDir         = filepath.GetDirectory();
    String fileName      = filepath.GetName();
    String fileExtension = filepath.GetExtension();

    Array<String> splitted = fileName.Split<Array>('_');
    int number = 1;
    if (splitted.Size() > 1)
    {
        String numberString = splitted[splitted.Size() - 1];
        bool ok = false;
        int readNumber = String::ToInt(numberString, &ok);
        if (ok)
        {
            number = readNumber + 1;
            splitted.PopBack();

            int lastUnderscorePos = fileName.RFind('_');
            if (lastUnderscorePos != -1) // Strip _[number] from fileName
            {
                fileName = fileName.SubString(0, lastUnderscorePos-1);
            }
        }
    }

    Path result = fileDir.Append(fileName + "_" + String::ToString(number))
                         .AppendExtension(fileExtension);
    return result;
}

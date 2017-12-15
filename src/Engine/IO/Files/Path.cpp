#include "Bang/Path.h"

#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Bang/List.h"
#include "Bang/Array.h"

USING_NAMESPACE_BANG

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
        m_absolutePath.Remove(m_absolutePath.Size()-1,
                              m_absolutePath.Size());
    }

    if (m_absolutePath.BeginsWith("./"))
    {
        m_absolutePath.Remove(0, 1);
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

List<Path> Path::FindFiles(FindFlags findFlags,
                           const List<String> &extensions) const
{
    if (!IsDir()) { return {}; }

    if (!findFlags.IsOn(FindFlag::Recursive))
    {
        List<Path> subFilesList;
        List<Path> subPathsList = FindSubPaths(findFlags);
        for (const Path &subPath : subPathsList)
        {
            bool extMatches = extensions.IsEmpty() ||
                              subPath.HasExtension(extensions);
            if ( subPath.IsFile() && extMatches )
            {
                subFilesList.PushBack(subPath);
            }
        }
        return subFilesList;
    }
    else
    {
        List<Path> filesList;
        FindFlags noRecursive = findFlags;
        noRecursive.SetOff(FindFlag::Recursive);
        filesList.PushBack( FindFiles(noRecursive, extensions) );

        List<Path> subDirs = FindSubDirectories(findFlags);
        for (const Path &subdir : subDirs)
        {
            List<Path> subFilesList = subdir.FindFiles(findFlags, extensions);
            filesList.PushBack(subFilesList);
        }
        return filesList;
    }
}

List<Path> Path::FindSubDirectories(FindFlags findFlags) const
{
    if (!IsDir()) { return {}; }

    if (findFlags.IsOff(FindFlag::Recursive))
    {
        List<Path> subDirsList;
        List<Path> subPathsList = FindSubPaths(findFlags);
        for (const Path &subPath : subPathsList)
        {
            if (subPath.IsDir()) { subDirsList.PushBack(subPath); }
        }
        return subDirsList;
    }
    else
    {
        FindFlags noRecursive = findFlags;
        noRecursive.SetOff(FindFlag::Recursive);
        List<Path> subdirsList = FindSubDirectories(noRecursive);
        for (Path subdirPath : subdirsList)
        {
            List<Path> subdirsListRecursive =
                    subdirPath.FindSubDirectories(findFlags);
            subdirsList.Splice(subdirsList.End(), subdirsListRecursive);
        }
        return subdirsList;
    }
}

List<Path> Path::FindSubPaths(FindFlags findFlags) const
{
    struct dirent *dir;
    DIR *d = opendir(GetAbsolute().ToCString());
    if (!d) { return {}; }

    List<Path> subPathsList;
    while ((dir = readdir(d)) != NULL)
    {
        String subName(dir->d_name);
        if (findFlags.IsOff(FindFlag::Hidden) &&
            (subName.BeginsWith("."))) { continue; }

        if (subName != "." && subName != "..")
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
    String name = "";
    bool iteratingFirstDots = true; // Treat hidden files "....foo.txt.bang"
    const String nameExt = GetNameExt();
    for (int i = 0; i < nameExt.Size(); ++i)
    {
        if (nameExt[i] != '.') { iteratingFirstDots = false; }
        if (iteratingFirstDots) { name += nameExt[i]; }
        else if (nameExt[i] != '.') { name += nameExt[i]; }
        else { break; }
    }
    return name;
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
    if (IsEmpty()) { return ""; }
    List<String> parts = GetNameExt().Split<List>('.');
    if (parts.IsEmpty()) { return ""; }
    parts.PopFront();
    return String::Join(parts, ".");
}

Array<String> Path::GetExtensions() const
{
    if (IsEmpty()) { return {}; }
    List<String> parts = GetNameExt().Split<List>('.');
    if (!parts.IsEmpty()) { parts.PopFront(); }
    return parts.To<Array>();
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
    if (str.BeginsWith("./")) { str.Remove(0, 1); }
    while (str.BeginsWith("/")) { str.Remove(0, 1); }
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
    if (HasExtension(extension) || extension.IsEmpty()) { return Path(*this); }
    return Path(GetAbsolute() + "." + extension);
}

bool Path::IsHiddenFile() const
{
    return IsFile() && GetName().BeginsWith(".");
}

Path Path::WithHidden(bool hidden) const
{
    String nameExt = GetNameExt();
    if ( hidden && !nameExt.BeginsWith(".")) { nameExt.Insert(0, "."); }
    if (!hidden &&  nameExt.BeginsWith(".")) { nameExt.Remove(0, 1); }
    return GetDirectory().Append(nameExt);
}

Path Path::WithNameExt(const String &name, const String &extension) const
{
    return GetDirectory().Append(name).AppendExtension(extension);
}

Path Path::WithExtension(const String &extension) const
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
    Array<String> thisExtensions = GetExtensions();
    for (const String &extension : extensions)
    {
        for (const String &thisExtension : thisExtensions)
        {
            if (extension.EqualsNoCase(thisExtension)) { return true; }
        }
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

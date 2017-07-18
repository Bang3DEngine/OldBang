#include "Path.h"

#include <QDir>
#include <QFile>

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
    return QFileInfo(GetAbsolute().ToQString()).isDir();
}

bool Path::IsFile() const
{
    return QFileInfo(GetAbsolute().ToQString()).isFile();
}

bool Path::Exists() const
{
    return IsDir() || IsFile();
}

List<Path> Path::GetFiles(bool recursive, const List<String> &extensions) const
{
    if (!IsDir()) { return {}; }

    QStringList extensionList;
    for (String ext : extensions)
    {
        if (!ext.BeginsWith("*.")) { ext.Prepend("*."); }
        extensionList.append(ext.ToQString());
    }

    List<Path> subdirs;
    if (recursive)
    {
        List<Path> subdirsRecursive = GetSubDirectories(recursive);
        subdirs.Splice(subdirs.Begin(), subdirsRecursive);
    }
    subdirs.PushFront(*this);

    List<Path> filesList;
    for (const Path &subdir : subdirs)
    {
        QStringList filepathList = QDir(subdir.GetAbsolute().ToQString())
                                   .entryList(extensionList);
        for (const QString &qFilepath : filepathList)
        {
            if (qFilepath == "." || qFilepath == "..") { continue; }

            Path filepath = Path(subdir).Append( String(qFilepath) );
            if (!filepath.IsFile()) { continue; }

            filesList.Add(filepath);
        }
    }
    return filesList;
}

List<Path> Path::GetSubDirectories(bool recursive) const
{
    if (!IsDir()) { return {}; }

    List<Path> subdirsList;
    QStringList subdirs =  QDir(GetAbsolute().ToQString()).entryList();
    for (QString qSubdir : subdirs)
    {
        if (qSubdir == "." || qSubdir == "..") { continue; }

        Path subdirPath = this->Append( String(qSubdir) );
        if (subdirPath.IsDir())
        {
            subdirsList.Add(subdirPath);
            if (recursive)
            {
                List<Path> subdirsListRecursive =
                        subdirPath.GetSubDirectories(recursive);
                subdirsList.Splice(subdirsList.End(), subdirsListRecursive);
            }
        }
    }
    return subdirsList;
}

String Path::GetName() const
{
    Array<String> parts = GetNameExt().Split('.');
    return parts.Size() >= 1 ? parts[0] : "";
}

String Path::GetNameExt() const
{
    if (IsEmpty()) { return ""; }

    String filename = GetAbsolute();
    const size_t lastSlash = GetAbsolute().rfind('/');
    if (lastSlash != String::npos)
    {
        filename = GetAbsolute().SubString(lastSlash + 1);
    }
    return filename;
}

String Path::GetExtension() const
{
    Array<String> parts = GetNameExt().Split('.');
    return parts.Size() >= 2 ? parts[1] : "";
}

Path Path::GetDirectory() const
{
    if (IsEmpty()) { return Path::Empty; }

    const size_t lastSlash = GetAbsolute().rfind('/');
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
    return GetAbsolute().Empty();
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
    return Path( GetDirectory().Append(GetName()).Append(extension) );
}

bool Path::HasExtension(const String &extensions) const
{
    List<String> extensionsList = extensions.Split(' ', true).To<List,String>();
    return HasExtension(extensionsList);
}

bool Path::HasExtension(const List<String> &extensions) const
{
    String ext = GetExtension();
    return extensions.Any( BPRED(x.EqualsNoCase(ext)) );
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

    Array<String> splitted = fileName.Split('_');
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

            int lastUnderscorePos = fileName.rfind('_');
            if (lastUnderscorePos != -1) // Strip _[number] from fileName
            {
                fileName = fileName.substr(0, lastUnderscorePos);
            }
        }
    }

    Path result = fileDir.Append(fileName + "_" + String::ToString(number))
                         .AppendExtension(fileExtension);
    return result;
}

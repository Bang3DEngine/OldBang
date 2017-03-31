#include "Bang/IO.h"

#include <QDir>
#include <QFile>
#include <ostream>
#include <fstream>
#include <QCryptographicHash>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/SingletonManager.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#endif

bool IO::IsDir(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isDir();
}

bool IO::IsFile(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isFile();
}

bool IO::IsAbsolute(const String &path)
{
	if (path.Empty()) { return false; }
    return QFileInfo(path.ToQString()).isAbsolute();
}

String IO::GetDir(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String directory = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        directory = filepath.substr(0, lastSlash);
    }
    return directory;
}

String IO::GetFileName(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String filename = filepath;
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    Array<String> parts = filename.Split('.');
    if (!parts.Empty())
    {
        filename = parts[0];
    }

    return filename;
}

String IO::GetFileNameWithExtension(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String filename = filepath;
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    return filename;
}

String IO::GetFileExtensionFirst(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[1];
}

String IO::GetFileExtensionLast(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[parts.Size() - 1];
}

String IO::GetFileContents(const String &filepath)
{
    if (!IO::ExistsFile(filepath)) { return ""; }

    String contents = "";
    std::ifstream ifs(filepath);
    if (ifs)
    {
        contents = String((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
        ifs.close();
    }
    else
    {
        Debug_Error("Can't open file '" << filepath << "': " << std::strerror(errno));
    }
    return contents;
}

String IO::GetFileExtensionComplete(const String &filepath)
{
    String fileNameWithExtension = IO::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    parts.Remove(parts.Begin());
    return String::Join(parts, ".");
}

String IO::GetPathWithoutExtension(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    String path = GetDir(filepath);
    path += GetFileName(filepath);
    return path;
}


String IO::ToAbsolute(const String &relPath,
                      const String &prependDirectory)
{
	if (relPath.Empty()) { return ""; }

    String result = "";
    if (IO::IsAbsolute(relPath))
    {
        result = relPath;
    }
    else
    {
        String pDir = prependDirectory;
        if (!pDir.Empty())
        {
            if (pDir[pDir.Length()-1] == '/')
            {
                pDir = pDir.SubString(1, pDir.Length()-1);
            }
        }

        String rPath = relPath;
        if (rPath[0] == '.' && rPath[1] == '/') // Something like "./Images/wololo"
        {
            return  pDir + "/" +
                    relPath.substr(2, relPath.Length() - 1);
        }

        result = pDir + "/" + relPath;
    }

    // If we try to get canonical for non-existing filepath,
    // then it returns empty string, we must handle this
    if (IO::Exists(result))
    {
        result = QFileInfo(result.ToQString()).canonicalFilePath();
    }

    return result.Replace("//", "/");
}

String IO::ToAbsolute(const String &relPath, bool isEngineFile)
{
	if (relPath.Empty()) { return ""; }
    return IO::ToAbsolute(relPath,
              isEngineFile ? IO::GetEngineAssetsRootAbs() :
                             IO::GetProjectAssetsRootAbs());
}

String IO::ToRelative(const String &absPath,
                               const String &prependDirectory)
{
	if (absPath.Empty()) { return ""; }

    if (!IsAbsolute(absPath))
    {
        if (absPath[0] != '.')
        {
            return "./" + absPath;
        }
        return absPath;
    }

    std::size_t pos = absPath.find(prependDirectory);
    if (pos == String::npos) return absPath;

    String relPath = "." + absPath.substr( pos + prependDirectory.Length(),
                                          (absPath.Length() - prependDirectory.Length()));
    return relPath;
}

String IO::ToRelative(const String &relPath, bool isEngineFile)
{
	if (relPath.Empty()) { return ""; }

    return IO::ToRelative(relPath,
              isEngineFile ? IO::GetEngineAssetsRootAbs() :
                             IO::GetProjectAssetsRootAbs());
}

String IO::ToRelative(const String &absPath)
{
	if (absPath.Empty()) { return ""; }
    return IO::ToRelative(absPath, IO::IsEngineFile(absPath));
}

bool IO::IsEngineFile(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    return filepath.BeginsWith(IO::GetEngineAssetsRootAbs());
}

String IO::GetDirUp(const String &filepath)
{
	if (filepath.Empty()) { return ""; }
    Array<String> splits = filepath.Split('/');
    splits.PopBack();
	String path = String::Join(splits, "/");
    return IO::IsAbsolute(filepath) ? IO::ToAbsolute(path, IO::IsEngineFile(filepath)) : path;
}

String IO::GetRightmostDir(const String &dir)
{
    Array<String> parts = dir.Split('/');
	if (parts.Empty()) { return ""; }
    return parts.Back();
}

bool IO::DuplicateFile(const String &fromFilepath,
                                const String &toFilepath,
                                bool overwrite)
{
	if (!IO::ExistsFile(fromFilepath)) { return false; }
    if (overwrite) { IO::Remove(toFilepath); }
    bool ok = QFile::copy(fromFilepath.ToQString(), toFilepath.ToQString());
    return ok;
}

bool IO::DuplicateDir(const String &fromDirpath,
                               const String &toDirpath,
                               bool overwrite)
{
	if (!IO::ExistsDirectory(fromDirpath)) { return false; }

    if (!IO::CreateDirectory(toDirpath)) { return false; }
    List<String> filepaths = IO::GetFiles(fromDirpath, false);
    for(const String & filepath : filepaths)
    {
        String fileName = IO::GetFileNameWithExtension(filepath);
        bool ok = IO::DuplicateFile(fromDirpath + "/" + fileName,
                                             toDirpath   + "/" + fileName,
                                             overwrite);
        if (!ok) { return false; }
    }

    List<String> subdirs = IO::GetSubDirectories(fromDirpath, false);
    for (const String &subdir : subdirs)
    {
        bool ok = IO::DuplicateDir(
                            subdir,
                            toDirpath + "/" + IO::GetRightmostDir(subdir),
                            overwrite);
        if (!ok) { return false; }
    }
    return true;
}

#ifdef BANG_EDITOR
String IO::GetNextDuplicatePath(const String &path)
{
	if (path.Empty()) { return ""; }

    String filePath = path;
    String fileDir  = IO::GetDir(filePath);
    String fileName = IO::GetFileName(filePath);
    String fileExtension = IO::GetFileExtensionComplete(path);

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

    String result = "";
    if (fileDir != "")
    {
        result += fileDir + "/";
    }
    result += fileName + "_" + std::to_string(number);
    if (fileExtension != "")
    {
        result += "." + fileExtension;
    }
    return result;
}

String IO::GetDuplicatePath(const String &path)
{
	if (path.Empty()) { return ""; }
    String result = path;
    while (IO::Exists(result))
    {
        result = IO::GetNextDuplicatePath(result);
    }
    return result;
}
#endif

List<String> IO::GetSubDirectories(const String &dirPath,
                                            bool recursive)
{
	if (dirPath.Empty()) { return {}; }
    List<String> subdirsList;
    if (!IO::ExistsDirectory(dirPath)) { return subdirsList; }

    QStringList subdirs =  QDir(dirPath.ToQString()).entryList();
    for (QString qSubdir : subdirs)
    {
        if (qSubdir == "." || qSubdir == "..") continue;

        String subdirName = String(qSubdir);
        String subdirPath = dirPath + "/" + subdirName;
        if (IO::IsDir(subdirPath))
        {
            subdirsList.Add(subdirPath);
            if (recursive)
            {
                List<String> subdirsListRecursive =
                        GetSubDirectories(subdirPath, recursive);
                subdirsList.Splice(subdirsList.End(), subdirsListRecursive);
            }
        }
    }
    return subdirsList;
}

List<String> IO::GetFiles(const String &dirPath,
                                   bool recursive,
                                   const List<String> &extensions)
{
	if (dirPath.Empty()) { return {}; }
    List<String> filesList;
    QStringList extensionList;
    for (String ext : extensions)
    {
        if (!ext.BeginsWith("*.")) { ext = "*." + ext; }
        extensionList.append(ext.ToQString());
    }

    List<String> subdirs;
    if (recursive)
    {
        List<String> subdirsRecursive = GetSubDirectories(dirPath, recursive);
        subdirs.Splice(subdirs.Begin(), subdirsRecursive);
    }
    subdirs.PushFront(dirPath);
    for (String subdir : subdirs)
    {
        QStringList filepathList = QDir(subdir.ToQString())
                .entryList(extensionList);

        for (QString qFilepath : filepathList)
        {
            String filepath(qFilepath);
            filepath = subdir + "/" + filepath;
            if (filepath == "." || filepath == ".." ||
                !IO::IsFile(filepath)) { continue; }

            filesList.Add(filepath);
        }
    }
    return filesList;
}

bool IO::Remove(const String &path)
{
	if (!IO::ExistsFile(path)) { return false; }
    if (IO::IsFile(path))
    {
        QFile f(path.ToQString());
        return f.remove();
    }
    else
    {
        List<String> subDirs  = GetSubDirectories(path, false);
        for (String subDir : subDirs) { IO::Remove(subDir); }
        List<String> subFiles = GetFiles(path, false);
        for (String subFile : subFiles) { IO::Remove(subFile); }
        QDir().rmdir(path.ToQString());
    }
    return false;
}

bool IO::ExistsFile(const String &filepath)
{
	if (filepath.Empty()) { return false; }
    return QFile(filepath.ToQString()).exists();
}

bool IO::ExistsDirectory(const String &dirPath)
{
	if (dirPath.Empty()) { return false; }
    return QDir(dirPath.ToQString()).exists();
}

bool IO::Exists(const String &filepath)
{
	if (filepath.Empty()) { return false; }
    if (IO::IsDir(filepath))
    {
        return IO::ExistsDirectory(filepath);
    }
    return IO::ExistsFile(filepath);
}

bool IO::CreateDirectory(const String &dirPath)
{
	if (dirPath.Empty()) { return false; }
    if (IO::ExistsDirectory(dirPath)) { return true; }
    return QDir().mkdir(dirPath.ToQString());
}

void IO::SetActiveSceneFilepath(const String &scenePath)
{
    IO::GetInstance()->c_currentSceneFilepath = scenePath;
}

bool IO::Rename(const String &oldPath, const String &newPath)
{
    return IO::Move(oldPath, newPath);
}

bool IO::Move(const String &oldPath, const String &newPath)
{
	if (!IO::Exists(oldPath)) { return false; }
    if (IO::IsDir(oldPath))
    {
        return QDir().rename(oldPath.ToQString(), newPath.ToQString());
    }
    return QFile().rename(oldPath.ToQString(), newPath.ToQString());
}

bool IO::WriteToFile(const String &absFilepath, const String &contents)
{
    std::ofstream out(absFilepath);
    if (out.is_open())
    {
        out << contents;
        out.close();
        return true;
    }
    return false;
}

String IO::GetHash(const String &filepath)
{
	if (!IO::ExistsFile(filepath)) { return ""; }

    String result = "";
    QFile file(filepath.ToQString());
    if(file.open(QIODevice::ReadOnly))
    {
        result = IO::GetHashFromByteArray(file.readAll());
        file.close();
    }
    return result;
}

String IO::GetHashFromString(const String &str)
{
    return IO::GetHashFromByteArray( str.ToQString().toUtf8() );
}

String IO::GetHashFromByteArray(const QByteArray &byteArray)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QByteArray hashBytes = hash.hash(byteArray, QCryptographicHash::Sha1);
    return String( QString(hashBytes.toHex()) );
}

String IO::AppendExtension(const String &filepath, const String extNoDot)
{
	if (filepath.Empty()) { return ""; }
    if (filepath.EndsWith("." + extNoDot)) { return filepath; }
    return filepath + "." + extNoDot;
}

void IO::InitFromMainBinary()
{
    SingletonManager::Set<IO>(new IO());
}

IO *IO::GetInstance()
{
    return SingletonManager::Get<IO>();
}

const String &IO::GetCurrentSceneFilepath()
{
    return IO::GetInstance()->c_currentSceneFilepath;
}
const String &IO::GetProjectRootAbs()
{
    return IO::GetInstance()->c_ProjectRootAbsolute;
}

String IO::GetProjectLibsRootAbs()
{
    return IO::GetInstance()->c_ProjectRootAbsolute + "/Libraries";
}
const String &IO::GetProjectAssetsRootAbs()
{
    return IO::GetInstance()->c_ProjectAssetsRootAbsolute;
}
const String &IO::GetEngineRootAbs()
{
    return IO::GetInstance()->c_EngineRootAbsolute;
}
const String &IO::GetEngineAssetsRootAbs()
{
    return IO::GetInstance()->c_EngineAssetsRootAbsolute;
}




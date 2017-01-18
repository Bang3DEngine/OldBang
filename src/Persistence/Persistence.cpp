#include "Persistence.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

#include <QDir>
#include <QFile>
#include <QCryptographicHash>

#include "Debug.h"
#include "StringUtils.h"
#include "SingletonManager.h"

bool Persistence::IsDir(const String &path)
{
    ASSERT(!path.Empty(), "", return false);
    return QFileInfo(path.ToQString()).isDir();
}

bool Persistence::IsFile(const String &path)
{
    ASSERT(!path.Empty(), "", return false);
    return QFileInfo(path.ToQString()).isFile();
}

bool Persistence::IsAbsolute(const String &path)
{
    ASSERT(!path.Empty(), "", return false);
    return QFileInfo(path.ToQString()).isAbsolute();
}

String Persistence::GetDir(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
    String directory = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        directory = filepath.substr(0, lastSlash);
    }
    return directory;
}

String Persistence::GetFileName(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
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

String Persistence::GetFileNameWithExtension(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
    String filename = filepath;
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    return filename;
}

String Persistence::GetFileExtensionFirst(const String &filepath)
{
    String fileNameWithExtension = Persistence::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[1];
}

String Persistence::GetFileExtensionLast(const String &filepath)
{
    String fileNameWithExtension = Persistence::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    return parts[parts.Size() - 1];
}

String Persistence::GetFileExtensionComplete(const String &filepath)
{
    String fileNameWithExtension = Persistence::GetFileNameWithExtension(filepath);
    Array<String> parts = fileNameWithExtension.Split('.');
    if (parts.Size() <= 1) { return ""; }
    parts.Remove(parts.Begin());
    return String::Join(parts, ".");
}

String Persistence::GetPathWithoutExtension(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
    String path = GetDir(filepath);
    path += GetFileName(filepath);
    return path;
}


String Persistence::ToAbsolute(const String &relPath,
                               const String &prependDirectory)
{
    ASSERT(!relPath.Empty(), "", return "");

    String result = "";
    if (Persistence::IsAbsolute(relPath))
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
    if (Persistence::Exists(result))
    {
        result = QFileInfo(result.ToQString()).canonicalFilePath();
    }

    return result;
}

String Persistence::ToAbsolute(const String &relPath, bool isEngineFile)
{
    ASSERT(!relPath.Empty(), "", return "");
    return Persistence::ToAbsolute(relPath,
              isEngineFile ? Persistence::GetEngineAssetsRootAbs() :
                             Persistence::GetProjectAssetsRootAbs());
}

String Persistence::ToRelative(const String &absPath,
                               const String &prependDirectory)
{
    ASSERT(!absPath.Empty(), "", return "");

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

String Persistence::ToRelative(const String &relPath, bool isEngineFile)
{
    ASSERT(!relPath.Empty(), "", return "");
    return Persistence::ToRelative(relPath,
              isEngineFile ? Persistence::GetEngineAssetsRootAbs() :
                             Persistence::GetProjectAssetsRootAbs());
}

String Persistence::ToRelative(const String &absPath)
{
    ASSERT(!absPath.Empty(), "", return "");
    return Persistence::ToRelative(absPath, Persistence::IsEngineFile(absPath));
}

bool Persistence::IsEngineFile(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
    return filepath.BeginsWith(Persistence::GetEngineAssetsRootAbs());
}

String Persistence::GetDirUp(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return "");
    Array<String> splits = filepath.Split('/');
    splits.PopBack();
    return (Persistence::IsAbsolute(filepath) ? "/" : "") +
            String::Join(splits, "/");
}

String Persistence::GetRightmostDir(const String &dir)
{
    Array<String> parts = StringUtils::Split(dir, '/');
    ASSERT(!parts.Empty(), "", return "");
    return parts.Back();
}

bool Persistence::DuplicateFile(const String &fromFilepath,
                                const String &toFilepath,
                                bool overwrite)
{
    ASSERT(Persistence::ExistsFile(fromFilepath), "", return false);
    if (overwrite) { Persistence::Remove(toFilepath); }
    bool ok = QFile::copy(fromFilepath.ToQString(), toFilepath.ToQString());
    return ok;
}

bool Persistence::DuplicateDir(const String &fromDirpath,
                               const String &toDirpath,
                               bool overwrite)
{
    ASSERT(Persistence::ExistsDirectory(fromDirpath), "", return false);

    if (!Persistence::CreateDirectory(toDirpath)) { return false; }
    List<String> filepaths = Persistence::GetFiles(fromDirpath, false);
    for(const String & filepath : filepaths)
    {
        String fileName = Persistence::GetFileNameWithExtension(filepath);
        bool ok = Persistence::DuplicateFile(fromDirpath + "/" + fileName,
                                             toDirpath   + "/" + fileName,
                                             overwrite);
        if (!ok) { return false; }
    }

    List<String> subdirs = Persistence::GetSubDirectories(fromDirpath, false);
    for (const String &subdir : subdirs)
    {
        bool ok = Persistence::DuplicateDir(
                            subdir,
                            toDirpath + "/" + Persistence::GetRightmostDir(subdir),
                            overwrite);
        if (!ok) { return false; }
    }
    return true;
}

#ifdef BANG_EDITOR
String Persistence::GetNextDuplicateName(const String &path)
{
    ASSERT(!path.Empty(), "", return "");

    String filePath = path;
    String fileDir  = Persistence::GetDir(filePath);
    String fileName = Persistence::GetFileNameWithExtension(filePath);

    Array<String> splitted = StringUtils::Split(fileName, '.');
    fileName = splitted[0];
    String fileExtension = splitted.Size() <= 1 ? "" : splitted[1];
    splitted = StringUtils::Split(fileName, '_');
    int number = 1;
    if (splitted.Size() > 1)
    {
        String numberString = splitted[splitted.Size() - 1];
        bool ok = false;
        int readNumber = StringUtils::ToInt(numberString, &ok);
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

String Persistence::GetDuplicateName(const String &path)
{
    ASSERT(!path.Empty(), "", return "");
    String result = path;
    while (Persistence::Exists(result))
    {
        result = Persistence::GetNextDuplicateName(result);
    }
    return result;
}
#endif

List<String> Persistence::GetSubDirectories(const String &dirPath,
                                            bool recursive)
{
    ASSERT(!dirPath.Empty(), "", return {});
    List<String> subdirsList;
    if (!Persistence::ExistsDirectory(dirPath)) { return subdirsList; }

    QStringList subdirs =  QDir(dirPath.ToQString()).entryList();
    for (QString qSubdir : subdirs)
    {
        if (qSubdir == "." || qSubdir == "..") continue;

        String subdirName = String(qSubdir);
        String subdirPath = dirPath + "/" + subdirName;
        if (Persistence::IsDir(subdirPath))
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

List<String> Persistence::GetFiles(const String &dirPath,
                                   bool recursive,
                                   const List<String> &extensions)
{
    ASSERT(!dirPath.Empty(), "", return {});
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
                !Persistence::IsFile(filepath)) { continue; }

            filesList.Add(filepath);
        }
    }
    return filesList;
}

bool Persistence::Remove(const String &path)
{
    ASSERT(Persistence::ExistsFile(path), "", return false);
    if (Persistence::IsFile(path))
    {
        QFile f(path.ToQString());
        return f.remove();
    }
    else
    {
        List<String> subDirs  = GetSubDirectories(path, false);
        for (String subDir : subDirs) { Persistence::Remove(subDir); }
        List<String> subFiles = GetFiles(path, false);
        for (String subFile : subFiles) { Persistence::Remove(subFile); }
        QDir().rmdir(path.ToQString());
    }
    return false;
}

bool Persistence::ExistsFile(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return false);
    return QFile(filepath.ToQString()).exists();
}

bool Persistence::ExistsDirectory(const String &dirPath)
{
    ASSERT(!dirPath.Empty(), "", return false);
    return QDir(dirPath.ToQString()).exists();
}

bool Persistence::Exists(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", return false);
    if (Persistence::IsDir(filepath))
    {
        return Persistence::ExistsDirectory(filepath);
    }
    return Persistence::ExistsFile(filepath);
}

bool Persistence::CreateDirectory(const String &dirPath)
{
    ASSERT(!dirPath.Empty(), "", return false);
    if (Persistence::ExistsDirectory(dirPath)) { return true; }
    return QDir().mkdir(dirPath.ToQString());
}

void Persistence::SetActiveSceneFilepath(const String &scenePath)
{
    Persistence::GetInstance()->c_currentSceneFilepath = scenePath;
}

bool Persistence::Rename(const String &oldPath, const String &newPath)
{
    ASSERT(Persistence::Exists(oldPath), "", return false);
    if (Persistence::IsDir(oldPath))
    {
        return QDir().rename(oldPath.ToQString(), newPath.ToQString());
    }
    else
    {
        return QFile().rename(oldPath.ToQString(), newPath.ToQString());
    }
}

bool Persistence::Move(const String &oldPath, const String &newPath)
{
    return Persistence::Rename(oldPath, newPath);
}

String Persistence::GetHash(const String &filepath)
{
    ASSERT(Persistence::ExistsFile(filepath), "", return "");

    QFile file(filepath.ToQString());
    if(file.open(QIODevice::ReadOnly))
    {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        QByteArray result = hash.hash(file.readAll(), QCryptographicHash::Sha1);
        file.close();
        return String( QString(result.toHex()) );
    }
    return "";
}

String Persistence::AppendExtension(const String &filepath, const String extNoDot)
{
    ASSERT(!filepath.Empty(), "", return "");
    if (filepath.find("." + extNoDot) != String::npos) return filepath;
    return filepath + "." + extNoDot;
}

void Persistence::InitFromMainBinary()
{
    SingletonManager::GetInstance()->SetPersistenceSingleton(new Persistence());
}

Persistence *Persistence::GetInstance()
{
    return SingletonManager::GetInstance()->GetPersistenceSingleton();
}

const String &Persistence::GetCurrentSceneFilepath()
{
    return Persistence::GetInstance()->c_currentSceneFilepath;
}
const String &Persistence::GetProjectRootAbs()
{
    return Persistence::GetInstance()->c_ProjectRootAbsolute;
}
const String &Persistence::GetProjectAssetsRootAbs()
{
    return Persistence::GetInstance()->c_ProjectAssetsRootAbsolute;
}
const String &Persistence::GetEngineRootAbs()
{
    return Persistence::GetInstance()->c_EngineRootAbsolute;
}
const String &Persistence::GetEngineAssetsRootAbs()
{
    return Persistence::GetInstance()->c_EngineAssetsRootAbsolute;
}




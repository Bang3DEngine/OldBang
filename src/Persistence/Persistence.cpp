#include "Persistence.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

#include <QDir>
#include <QFile>

#include "Debug.h"
#include "StringUtils.h"

String Persistence::s_currentSceneFilepath = "";

// Set by main.cpp
String Persistence::c_ProjectRootAbsolute = "";
String Persistence::c_ProjectAssetsRootAbsolute = "";
String Persistence::c_EngineRootAbsolute = "";
String Persistence::c_EngineAssetsRootAbsolute = "";

bool Persistence::IsDir(const String &path)
{
    return QFileInfo(path.ToQString()).isDir();
}

bool Persistence::IsFile(const String &path)
{
    return QFileInfo(path.ToQString()).isFile();
}

bool Persistence::IsAbsolute(const String &path)
{
    return QFileInfo(path.ToQString()).isAbsolute();
}

String Persistence::GetProjectRootPathAbsolute()
{
    return Persistence::c_ProjectRootAbsolute;
}

String Persistence::GetAssetsPathAbsolute()
{
    return Persistence::c_ProjectAssetsRootAbsolute;
}

String Persistence::GetDir(const String &filepath)
{
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
    String filename = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
        filename = StringUtils::Split(filename, '.')[0];
    }
    return filename;
}

String Persistence::GetFileNameWithExtension(const String &filepath)
{
    String filename = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != String::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    return filename;
}

String Persistence::GetPathWithoutExtension(const String &filepath)
{
    String path = GetDir(filepath);
    path += GetFileName(filepath);
    return path;
}


String Persistence::ToAbsolute(const String &relPath,
                               const String &prependDirectory)
{
    if (relPath == "") return "";
    if (IsAbsolute(relPath)) return relPath;

    String pDir = prependDirectory;
    if (pDir[pDir.Length()-1] == '/')
    {
        pDir = pDir.SubString(1, pDir.Length()-1);
    }

    String rPath = relPath;
    if (rPath[0] == '.' && rPath[1] == '/') // Something like "./Images/wololo"
    {
        return  prependDirectory + "/" +
                relPath.substr(2, relPath.Length() - 1);
    }

    return pDir + "/" + relPath;
}

String Persistence::ToAbsolute(const String &relPath, bool isEngineFile)
{
    return Persistence::ToAbsolute(relPath,
              isEngineFile ? Persistence::c_EngineAssetsRootAbsolute :
                             Persistence::c_ProjectAssetsRootAbsolute);
}

String Persistence::ToRelative(const String &absPath,
                               const String &prependDirectory)
{
    if (absPath.Empty()) return "";

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
    return Persistence::ToRelative(relPath,
              isEngineFile ? Persistence::c_EngineAssetsRootAbsolute :
                             Persistence::c_ProjectAssetsRootAbsolute);
}

String Persistence::ToRelative(const String &absPath)
{
    return Persistence::ToRelative(absPath, Persistence::IsEngineFile(absPath));
}

bool Persistence::IsEngineFile(const String &filepath)
{
    return filepath.BeginsWith(Persistence::c_EngineAssetsRootAbsolute);
}

String Persistence::GetDirUp(const String &filepath)
{
    Array<String> splits = filepath.Split('/');
    splits.PopBack();
    return (Persistence::IsAbsolute(filepath) ? "/" : "") +
            String::Join(splits, "/");
}

#ifdef BANG_EDITOR
String Persistence::GetNextDuplicateName(const String &path)
{
    String filePath = Persistence::ToRelative(path);
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

String Persistence::GetDuplicateName(const String &path, Explorer *exp)
{
    String result = path;
    while (exp->Exists(result))
    {
        result = Persistence::GetNextDuplicateName(result);
    }
    return result;
}

List<String> Persistence::GetSubDirectories(const String &dirPath,
                                            bool recursive)
{
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
    List<String> filesList;
    QStringList extensionList;
    for (String ext : extensions)
    {
        if (!ext.BeginsWith("*.")) { ext = "*." + ext; }
        extensionList.append(ext.ToQString());
    }

    List<String> subdirs = GetSubDirectories(dirPath, recursive);
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

bool Persistence::ExistsFile(const String &filepath)
{
    return QFile(filepath.ToQString()).exists();
}
#endif

bool Persistence::ExistsDirectory(const String &dirPath)
{
    return QDir(dirPath.ToQString()).exists();
}

bool Persistence::CreateDirectory(const String &dirPath)
{
    return QDir().mkdir(dirPath.ToQString());
}

void Persistence::SetActiveSceneFilepath(const String &scenePath)
{
    s_currentSceneFilepath = scenePath;
}

String Persistence::SetActiveSceneFilepath()
{
    return s_currentSceneFilepath;
}

String Persistence::AppendExtension(const String &filepath, const String extNoDot)
{
    if (filepath.find("." + extNoDot) != String::npos) return filepath;
    return filepath + "." + extNoDot;
}




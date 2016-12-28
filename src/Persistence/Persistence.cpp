#include "Persistence.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

#include "Debug.h"
#include "StringUtils.h"

String Persistence::s_currentSceneFilepath = "";

// Set by main.cpp
String Persistence::c_ProjectRootAbsolute = "";
String Persistence::c_ProjectAssetsRootAbsolute = "";
String Persistence::c_EngineAssetsRootAbsolute = "";

bool Persistence::IsAbsolute(const String &path)
{
    return path[0] == '/';
}

String Persistence::GetProjectRootPathAbsolute()
{
    return QT_PROJECT_PATH;
}

String Persistence::GetAssetsRelativePathFromProjectRoot()
{
    return "Assets";
}

String Persistence::GetAssetsPathAbsolute()
{
    return GetProjectRootPathAbsolute() + "/" +
           GetAssetsRelativePathFromProjectRoot();
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

    String rPath = relPath;
    if (rPath[0] == '.' && rPath[1] == '/') // Something like "./Images/wololo"
    {
        return  prependDirectory +
                relPath.substr(1, relPath.Length() - 2);
    }

    String pDir = prependDirectory;
    if (pDir[pDir.Length()-1] == '/')
    {
        pDir = pDir.SubString(0, pDir.Length()-1);
    }

    return pDir + "/" + relPath;
}

String Persistence::ToAbsolute(const String &relPath, bool isEngineFile)
{
    Debug_Log("ToAbsolute of " << relPath << ": " <<
              Persistence::ToAbsolute(relPath,
                 isEngineFile ? Persistence::c_EngineAssetsRootAbsolute :
                                Persistence::c_ProjectAssetsRootAbsolute)
              );
    return Persistence::ToAbsolute(relPath,
              isEngineFile ? Persistence::c_EngineAssetsRootAbsolute :
                             Persistence::c_ProjectAssetsRootAbsolute);
}

String Persistence::ToRelative(const String &absPath,
                               const String &prependDirectory)
{
    if (absPath == "") return "";

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

    return "." + absPath.substr(pos,
                                absPath.Length() - prependDirectory.Length());
}

String Persistence::ToRelative(const String &relPath, bool isEngineFile)
{
    return Persistence::ToRelative(relPath,
              isEngineFile ? Persistence::c_EngineAssetsRootAbsolute :
                             Persistence::c_ProjectAssetsRootAbsolute);
}

#ifdef BANG_EDITOR
String Persistence::GetNextDuplicateName(const String &path)
{
    String filePath = Persistence::ToRelative(path, false);
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
#endif

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




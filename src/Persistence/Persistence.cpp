#include "Persistence.h"

#include "Explorer.h"
#include "StringUtils.h"

String Persistence::s_currentSceneFilepath = "";

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
    return "/Assets";
}

String Persistence::GetAssetsPathAbsolute()
{
    return GetProjectRootPathAbsolute() +
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


String Persistence::ToAbsolute(const String &relPath)
{
    if (relPath == "") return "";
    if (IsAbsolute(relPath)) return relPath;

    if (relPath[0] == '.')
    {
        return GetProjectRootPathAbsolute() +
                relPath.substr(1, relPath.Length() - 1); //No starting "."
    }
    else
    {
        if (relPath[0] == '/')
        {
            return GetProjectRootPathAbsolute() + relPath;
        }
        else
        {
            return GetProjectRootPathAbsolute() + "/" + relPath;
        }
    }
}

String Persistence::ToRelative(const String &absPath)
{
    // /home/wololo/MyProject/Assets/lolol/a.bmesh => ./Assets/lolol/a.bmesh
    if (absPath == "") return "";

    if (!IsAbsolute(absPath))
    {
        if (absPath[0] != '.')
        {
            return "./" + absPath;
        }
        return absPath;
    }

    std::size_t pos = absPath.find(GetAssetsRelativePathFromProjectRoot());
    if (pos == String::npos) return absPath;

    return "." + absPath.substr(
                pos,
                absPath.Length() -
                GetAssetsRelativePathFromProjectRoot().Length());
}

String Persistence::GetNextDuplicateName(const String &path)
{
    String filePath = Persistence::ToRelative(path);
    String fileDir  = Persistence::GetDir(filePath);
    String fileName = Persistence::GetFileNameWithExtension(filePath);

    std::vector<String> splitted = StringUtils::Split(fileName, '.');
    fileName = splitted[0];
    String fileExtension = splitted.size() <= 1 ? "" : splitted[1];
    splitted = StringUtils::Split(fileName, '_');
    int number = 1;
    if (splitted.size() > 1)
    {
        String numberString = splitted[splitted.size() - 1];
        bool ok = false;
        int readNumber = StringUtils::ToInt(numberString, &ok);
        if (ok)
        {
            number = readNumber + 1;
            splitted.pop_back();

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




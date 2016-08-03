#include "Persistence.h"

std::string Persistence::s_currentSceneFilepath = "";

bool Persistence::IsAbsolute(const std::string &path)
{
    return path[0] == '/';
}

std::string Persistence::GetProjectRootPathAbsolute()
{
    return QT_PROJECT_PATH;
}

std::string Persistence::GetAssetsRelativePathFromProjectRoot()
{
    return "/Assets";
}

std::string Persistence::GetAssetsPathAbsolute()
{
    return GetProjectRootPathAbsolute() +
           GetAssetsRelativePathFromProjectRoot();
}

std::string Persistence::GetDir(const std::string &filepath)
{
    std::string directory = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != std::string::npos)
    {
        directory = filepath.substr(0, lastSlash);
    }
    return directory;
}

std::string Persistence::GetFileNameWithExtension(const std::string &filepath)
{
    std::string filename = "";
    const size_t lastSlash = filepath.rfind('/');
    if (lastSlash != std::string::npos)
    {
        filename = filepath.substr(lastSlash + 1);
    }
    return filename;
}


std::string Persistence::ProjectRootRelativeToAbsolute(const std::string &relPath)
{
    if (relPath == "") return "";
    if (IsAbsolute(relPath)) return relPath;

    if (relPath[0] == '.')
    {
        return GetProjectRootPathAbsolute() +
                relPath.substr(1, relPath.length() -1); //No starting "."
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

std::string Persistence::ProjectRootAbsoluteToRelative(const std::string &absPath)
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
    if (pos == std::string::npos) return absPath;

    return "." + absPath.substr(
                pos,
                absPath.length() -
                GetAssetsRelativePathFromProjectRoot().length());
}

void Persistence::SetCurrentSceneFilepath(const std::string &scenePath)
{
    s_currentSceneFilepath = scenePath;
}

std::string Persistence::GetCurrentSceneFilepath()
{
    return s_currentSceneFilepath;
}

std::string Persistence::AppendExtension(const std::string &filepath, const std::string extNoDot)
{
    if (filepath.find("." + extNoDot) != std::string::npos) return filepath;
    return filepath + "." + extNoDot;
}




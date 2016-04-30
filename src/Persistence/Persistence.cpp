#include "Persistence.h"

std::string Persistence::currentStageFilepath = "";

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
    return "/res/Assets";
}

std::string Persistence::GetAssetsPathAbsolute()
{
    return GetProjectRootPathAbsolute() +
            GetAssetsRelativePathFromProjectRoot();
}


std::string Persistence::ProjectRootRelativeToAbsolute(const std::string &relPath)
{
    if(IsAbsolute(relPath)) return relPath;

    return GetProjectRootPathAbsolute() +
            relPath.substr(1, relPath.length() -1); //No starting "."
}

std::string Persistence::ProjectRootAbsoluteToRelative(const std::string &absPath)
{
    // /home/wololo/MyProject/res/Assets/lolol/a.bmesh => ./res/Assets/lolol/a.bmesh
    if(!IsAbsolute(absPath) && absPath[0] == '.')
    {
        return absPath;
    }

    std::size_t pos = absPath.find(GetAssetsRelativePathFromProjectRoot());
    if(pos == std::string::npos) return absPath;

    return "." + absPath.substr(
                pos,
                absPath.length() -
                GetAssetsRelativePathFromProjectRoot().length());
}

void Persistence::SetCurrentStageFilepath(const std::string &stagePath)
{
    currentStageFilepath = stagePath;
}

std::string Persistence::GetCurrentStageFilepath()
{
    return currentStageFilepath;
}

std::string Persistence::AppendExtension(const std::string &filepath, const std::string extNoDot)
{
    if(filepath.find("." + extNoDot) != std::string::npos) return filepath;
    return filepath + "." + extNoDot;
}




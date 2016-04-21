#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>

class Persistence
{
public:

    static std::string GetProjectRootPathAbsolute()
    {
        return QT_PROJECT_PATH;
    }

    static std::string GetAssetsRelativePathFromProjectRoot()
    {
        return "/res/Assets";
    }

    static std::string GetAssetsPathAbsolute()
    {
        return GetProjectRootPathAbsolute() +
                GetAssetsRelativePathFromProjectRoot();
    }

    static bool IsAbsolute(const std::string &path)
    {
        return path[0] == '/';
    }

    // ./res/Assets/lolol/a.bmesh => /home/wololo/MyProject/res/Assets/lolol/a.bmesh
    static std::string ProjectRootRelativeToAbsolute(const std::string &relPath)
    {
        if(IsAbsolute(relPath)) return relPath;

        return GetProjectRootPathAbsolute() +
                relPath.substr(1, relPath.length() -1); //No beginning "."
    }

    // /home/wololo/MyProject/res/Assets/lolol/a.bmesh => ./res/Assets/lolol/a.bmesh
    static std::string ProjectRootAbsoluteToRelative(const std::string &absPath)
    {
        if(!IsAbsolute(absPath)) return absPath;

        std::size_t pos = absPath.find(GetAssetsRelativePathFromProjectRoot());
        if(pos == std::string::npos) return absPath;

        return "." + absPath.substr(
                        pos,
                        absPath.length() -
                                GetAssetsRelativePathFromProjectRoot().length());
    }
};

#endif // PERSISTENCE_H

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>

class Persistence
{

private:
   static std::string currentStageFilepath;

public:

    static bool IsAbsolute(const std::string &path);

    static std::string GetProjectRootPathAbsolute();

    static std::string GetAssetsRelativePathFromProjectRoot();

    static std::string GetAssetsPathAbsolute();


    // ./res/Assets/lolol/a.bmesh => /home/wololo/MyProject/res/Assets/lolol/a.bmesh
    static std::string ProjectRootRelativeToAbsolute(const std::string &relPath);

    // /home/wololo/MyProject/res/Assets/lolol/a.bmesh => ./res/Assets/lolol/a.bmesh
    static std::string ProjectRootAbsoluteToRelative(const std::string &absPath);


    static void SetCurrentStageFilepath(const std::string &stagePath);
    static std::string GetCurrentStageFilepath();
};

#endif // PERSISTENCE_H

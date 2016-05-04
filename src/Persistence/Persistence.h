#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>

class Persistence
{

private:
   static std::string currentSceneFilepath;

public:

    static bool IsAbsolute(const std::string &path);

    static std::string GetProjectRootPathAbsolute();

    static std::string GetAssetsRelativePathFromProjectRoot();

    static std::string GetAssetsPathAbsolute();


    // ./res/Assets/lolol/a.bmesh => /home/wololo/MyProject/res/Assets/lolol/a.bmesh
    static std::string ProjectRootRelativeToAbsolute(const std::string &relPath);

    // /home/wololo/MyProject/res/Assets/lolol/a.bmesh => ./res/Assets/lolol/a.bmesh
    static std::string ProjectRootAbsoluteToRelative(const std::string &absPath);


    static void SetCurrentSceneFilepath(const std::string &scenePath);
    static std::string GetCurrentSceneFilepath();

    //Appends an extension to a filepath only if that filename does not contain the extension
    static std::string AppendExtension(const std::string &filepath, const std::string extNoDot);
};

#endif // PERSISTENCE_H

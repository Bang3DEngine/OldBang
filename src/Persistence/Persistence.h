#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>

class Persistence
{

private:
   static std::string s_currentSceneFilepath;

public:

    static bool IsAbsolute(const std::string &path);

    static std::string GetProjectRootPathAbsolute();

    static std::string GetAssetsRelativePathFromProjectRoot();

    static std::string GetAssetsPathAbsolute();

    static std::string GetDir(const std::string &filepath);
    static std::string GetFileName(const std::string &filepath);
    static std::string GetFileNameWithExtension(const std::string &filepath);

    // ./Assets/lolol/a.bmesh => /home/wololo/MyProject/Assets/lolol/a.bmesh
    static std::string ToAbsolute(const std::string &relPath);

    // /home/wololo/MyProject/Assets/lolol/a.bmesh => ./Assets/lolol/a.bmesh
    static std::string ToRelative(const std::string &absPath);

    /**
     * @brief wololo/omg/myFile.jpg => wololo/omg/myFile_1.jpg
     *        wololo/omg/myFile_2.jpg => wololo/omg/myFile_3.jpg
     *        etc.
     * @param path
     * @return
     */
    static std::string GetNextDuplicateName(const std::string &path);

    static void SetCurrentSceneFilepath(const std::string &scenePath);
    static std::string GetCurrentSceneFilepath();

    //Appends an extension to a filepath only if that filename does not contain the extension
    static std::string AppendExtension(const std::string &filepath, const std::string extNoDot);
};

#endif // PERSISTENCE_H

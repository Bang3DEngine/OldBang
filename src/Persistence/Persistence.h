#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "Bang.h"

class Explorer;
class Persistence
{

private:
   static String s_currentSceneFilepath;

public:

    static bool IsAbsolute(const String &path);

    static String GetProjectRootPathAbsolute();

    static String GetAssetsRelativePathFromProjectRoot();

    static String GetAssetsPathAbsolute();

    static String GetDir(const String &filepath);
    static String GetFileName(const String &filepath);
    static String GetFileNameWithExtension(const String &filepath);
    static String GetPathWithoutExtension(const String &filepath);

    // ./Assets/lolol/a.bmesh => /home/wololo/MyProject/Assets/lolol/a.bmesh
    static String ToAbsolute(const String &relPath);

    // /home/wololo/MyProject/Assets/lolol/a.bmesh => ./Assets/lolol/a.bmesh
    static String ToRelative(const String &absPath);

    #ifdef BANG_EDITOR
    /**
     * @brief wololo/omg/myFile.jpg => wololo/omg/myFile_1.jpg
     *        wololo/omg/myFile_2.jpg => wololo/omg/myFile_3.jpg
     *        etc.
     * @param path
     * @return
     */
    static String GetNextDuplicateName(const String &path);
    static String GetDuplicateName(const String &path, Explorer *exp);
    #endif

    static void SetActiveSceneFilepath(const String &scenePath);
    static String SetActiveSceneFilepath();

    //Appends an extension to a filepath only if that filename does not contain the extension
    static String AppendExtension(const String &filepath, const String extNoDot);
};

#endif // PERSISTENCE_H

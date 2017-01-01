#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "List.h"
#include "String.h"

class Explorer;
class Persistence
{

private:
   static String s_currentSceneFilepath;

public:
   static String c_ProjectRootAbsolute;
   static String c_ProjectAssetsRootAbsolute;
   static String c_EngineRootAbsolute;
   static String c_EngineAssetsRootAbsolute;

   static bool IsDir(const String &path);
   static bool IsFile(const String &path);
   static bool IsAbsolute(const String &path);

   static String GetProjectRootPathAbsolute();
   static String GetAssetsPathAbsolute();

   static String GetDir(const String &filepath);
   static String GetFileName(const String &filepath);
   static String GetFileNameWithExtension(const String &filepath);
   static String GetPathWithoutExtension(const String &filepath);

   /**
    * ./Images/wololo => ${prependDirectory}/Images/wololo
    *   Images/wololo => ${prependDirectory}/Images/wololo
   **/
   static String ToAbsolute(const String &relPath,
                            const String &prependDirectory);
   static String ToAbsolute(const String &relPath,
                            bool isEngineFile);

   /**
    * ${prependDirectory}/Images/wololo => ./Images/wololo
   **/
   static String ToRelative(const String &absPath,
                            const String &prependDirectory);
   static String ToRelative(const String &relPath,
                            bool isEngineFile);
   static String ToRelative(const String &absPath);

   static bool IsEngineFile(const String &filepath);

   static String GetDirUp(const String &filepath);

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

   static List<String> GetSubDirectories(const String& dirPath,
                                         bool recursive = false);
   static List<String> GetFiles(const String& dirPath,
                                bool recursive = false,
                                List<String> extensions = {});

   static bool ExistsFile(const String &filepath);
   static bool ExistsDirectory(const String &dirPath);
   static bool CreateDirectory(const String &dirPath);
   static void SetActiveSceneFilepath(const String &scenePath);
   static String SetActiveSceneFilepath();

   //Appends an extension to a filepath only if that filename does not contain the extension
   static String AppendExtension(const String &filepath, const String extNoDot);
};

#endif // PERSISTENCE_H

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QByteArray>
#include "Bang/WinUndef.h"

#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/String.h"

class Explorer;
class IO
{
public:
   static bool IsEngineFile(const String &filepath);
   static void SetActiveSceneFilepath(const String &scenePath);

   static void InitFromMainBinary();

   static IO *GetInstance();
   static const String& GetProjectRootAbs();
   static String GetProjectLibsRootAbs();
   static const String& GetProjectAssetsRootAbs();
   static const String& GetEngineRootAbs();
   static const String& GetEngineAssetsRootAbs();

private:
   String c_ProjectRootAbsolute       = "";
   String c_ProjectAssetsRootAbsolute = "";
   String c_EngineRootAbsolute        = "";
   String c_EngineAssetsRootAbsolute  = "";

   friend class ProjectManager;
   friend int main(int argc, char** argv);
};

#endif // PERSISTENCE_H

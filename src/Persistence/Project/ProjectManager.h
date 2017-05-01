#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/Path.h"
#include "Bang/String.h"

class Project;
class ProjectManager
{
private:
    static Project *s_currentProject;

public:
    ProjectManager();

    static Project* OpenProject(const Path &projectFilepath);

    #ifdef BANG_EDITOR
    static Project* CreateNewProject(const Path &projectDirPath,
                                     const String &projectName);
    static Project* CreateNewProjectFileOnly(const Path &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static Path DialogCreateNewProject();
    static Path DialogOpenProject();
    #endif

    static Project* GetCurrentProject();
};

#endif // PROJECTMANAGER_H

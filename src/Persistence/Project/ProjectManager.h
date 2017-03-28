#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/String.h"

class Project;
class ProjectManager
{
private:
    static Project *s_currentProject;

public:
    ProjectManager();

    static Project* OpenProject(const String &projectFilepath);

    #ifdef BANG_EDITOR
    static Project* CreateNewProject(const String &projectContainingDir,
                                     const String &projectName);
    static Project* CreateNewProjectFileOnly(const String &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static String DialogCreateNewProject();
    static String DialogOpenProject();
    #endif

    static Project* GetCurrentProject();
};

#endif // PROJECTMANAGER_H

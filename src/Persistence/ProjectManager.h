#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "String.h"

class Project;
class ProjectManager
{
private:
    static Project *s_currentProject;

public:
    ProjectManager();

    static Project* CreateNewProject(const String &projectContainingDir,
                               const String &projectName);
    static Project* OpenProject(const String &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static String DialogCreateNewProject();
    static String DialogOpenProject();

    static Project* GetCurrentProject();
};

#endif // PROJECTMANAGER_H

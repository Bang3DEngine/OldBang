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

    static Project* NewProject(const String &projectContainingDir,
                               const String &projectName);
    static Project* OpenProject(const String &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static String CreateNewProjectAndDialogs();
    static String OpenProjectAndDialogs();

    static Project* GetCurrentProject();
};

#endif // PROJECTMANAGER_H

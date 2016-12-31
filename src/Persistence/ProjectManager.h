#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "String.h"

class Project;
class ProjectManager
{
private:
    static Project *currentProject;

public:
    ProjectManager();

    static Project* NewProject(const String &projectContainingDir,
                               const String &projectName);
    static Project* OpenProject(const String &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static Project* GetCurrentProject();
};

#endif // PROJECTMANAGER_H

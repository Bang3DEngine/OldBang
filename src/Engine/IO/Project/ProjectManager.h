#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang.h"

FORWARD class Path;
FORWARD class String;
FORWARD class Project;

class ProjectManager
{
public:
    ProjectManager();

    virtual Project* OpenProject(const Path &projectFilepath) const;

    static Project* CreateNewProject(const Path &projectDirPath,
                                     const String &projectName);
    static Project* CreateNewProjectFileOnly(const Path &projectFilepath);
    static void SaveProject(const Project *project);
    static void SaveCurrentProject();
    static void CloseCurrentProject();

    static Project* GetCurrentProject();

private:
    static Project *s_currentProject;
};

#endif // PROJECTMANAGER_H

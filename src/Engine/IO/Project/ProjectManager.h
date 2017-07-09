#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/Path.h"
#include "Bang/String.h"

class Project;
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

protected:
    virtual bool OpenFirstFoundScene(const Path &projectDirPath) const;

private:
    static Project *s_currentProject;
};

#endif // PROJECTMANAGER_H

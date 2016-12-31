#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "String.h"

class Project;
class ProjectManager
{
public:
    ProjectManager();

    static Project* NewProject(const String &projectContainingDir,
                               const String &projectName);
    static Project* OpenProject(const String &projectFilepath);
    static void SaveProject(const Project &project,
                            const String &projectFilepath);
};

#endif // PROJECTMANAGER_H

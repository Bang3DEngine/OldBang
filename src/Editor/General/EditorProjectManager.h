#ifndef EDITORPROJECTMANAGER_H
#define EDITORPROJECTMANAGER_H

#include "Bang/Path.h"
#include "Bang/ProjectManager.h"

class Project;
class EditorProjectManager : public ProjectManager
{
public:
    EditorProjectManager();

    static Path DialogCreateNewProject();
    static Path DialogOpenProject();

    Project* OpenProject(const Path &projectFilepath) const override;

protected:
    virtual bool OpenFirstFoundScene(const Path &projectDirPath) const;
};

#endif // EDITORPROJECTMANAGER_H

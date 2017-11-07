#include "Bang/ProjectManager.h"

#include <string>

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Project.h"
#include "Bang/XMLNode.h"
#include "Bang/XMLParser.h"
#include "Bang/Extensions.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Project *ProjectManager::s_currentProject = nullptr;

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::OpenProject(const Path &projectFilepath)
{
    ProjectManager::CloseCurrentProject();

    ProjectManager::s_currentProject = new Project();
    Project *currentProject = ProjectManager::s_currentProject;
    currentProject->ImportXMLFromFile(projectFilepath);
    currentProject->SetProjectRootFilepath( projectFilepath.GetDirectory() );

    Paths::SetProjectRoot(currentProject->GetProjectDirPath());

    ImportFilesManager::CreateMissingProjectImportFiles();
    ImportFilesManager::LoadImportFilepathGUIDs();

    return currentProject;
}

Project* ProjectManager::CreateNewProject(const Path &projectDirPath,
                                          const String &projectName)
{
    Path projectDir(projectDirPath + "/" + projectName);
    if (!projectDir.Exists())
    {
        if (!File::CreateDirectory(projectDir))
        {
            Debug_Error ("Could not create project in directory '" <<
                         projectDir << "'.");
            return nullptr;
        }
    }
    else
    {
        // TODO: Add overwrite window warning
        Debug_Warn("Directory '" << projectDir <<
                   "' already existed, using it.");
    }

    ProjectManager::CloseCurrentProject();

    Path projectFileFilepath(projectDir + "/" + projectName + "." +
                             Extensions::GetProjectExtension());

    ProjectManager::s_currentProject =
            CreateNewProjectFileOnly(projectFileFilepath);
    ProjectManager::s_currentProject->SetProjectRootFilepath(projectDir);

    File::CreateDirectory(projectDir.Append("Assets"));

    return ProjectManager::s_currentProject;
}

Project *ProjectManager::CreateNewProjectFileOnly(const Path &projectFilepath)
{
    Project *proj = new Project();
    proj->ExportXMLToFile(projectFilepath);
    return proj;
}

void ProjectManager::ExportProject(const Project *project)
{
    ENSURE(project);
    bool ok = project->ExportXMLToFile(project->GetProjectFileFilepath());
    if (ok)
    {
        Debug_Log("Project '" << project->GetProjectName() <<
                  "' successfully saved.");
    }
    else
    {
        Debug_Error("Could not save the project...");
    }
}

void ProjectManager::ExportCurrentProject()
{
    ProjectManager::ExportProject( ProjectManager::GetCurrentProject() );
}

void ProjectManager::CloseCurrentProject()
{
    if (ProjectManager::s_currentProject)
    {
        delete s_currentProject;
        ProjectManager::s_currentProject = nullptr;
    }

}

Project *ProjectManager::GetCurrentProject()
{
    return ProjectManager::s_currentProject;
}

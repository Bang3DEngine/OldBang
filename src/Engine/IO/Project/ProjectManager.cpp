#include "Bang/ProjectManager.h"

#include <string>

#include "Bang/File.h"
#include "Bang/List.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/XMLNode.h"
#include "Bang/XMLParser.h"
#include "Bang/Extensions.h"
#include "Bang/SceneManager.h"

Project *ProjectManager::s_currentProject = nullptr;

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::OpenProject(const Path &projectFilepath) const
{
    XMLNode xmlInfo = XMLParser::FromFile(projectFilepath);

    CloseCurrentProject();

    ProjectManager::s_currentProject = new Project();
    Project *currentProject = ProjectManager::s_currentProject;
    currentProject->Read(xmlInfo);
    currentProject->SetProjectRootFilepath( projectFilepath.GetDirectory() );

    Paths::SetProjectPath(currentProject->GetProjectDirPath());

    OpenFirstFoundScene(currentProject->GetProjectDirPath());

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
                             Extensions::Get<Project>());

    ProjectManager::s_currentProject =
            CreateNewProjectFileOnly(projectFileFilepath);
    ProjectManager::s_currentProject->SetProjectRootFilepath(projectDir);

    File::CreateDirectory(projectDir.Append("Assets"));

    return ProjectManager::s_currentProject;
}

Project *ProjectManager::CreateNewProjectFileOnly(const Path &projectFilepath)
{
    Project *proj = new Project();
    proj->WriteToFile(projectFilepath);
    return proj;
}

void ProjectManager::SaveProject(const Project *project)
{
    ENSURE(project);
    bool ok = project->WriteToFile(project->GetProjectFileFilepath());
    if (ok)
    {
        Debug_Status("Project '" << project->GetProjectName() <<
                     "' successfully saved.", 5.0f);
    }
    else
    {
        Debug_Error("Could not save the project...");
    }
}

void ProjectManager::SaveCurrentProject()
{
    ProjectManager::SaveProject( ProjectManager::GetCurrentProject() );
}

void ProjectManager::CloseCurrentProject()
{
    // TODO: Ask for saving current open project and stuff
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

bool ProjectManager::OpenFirstFoundScene(const Path &projectDirPath) const
{
    List<Path> sceneFilepaths = projectDirPath.GetFiles(true,
                                   {"*." + Extensions::Get<Scene>()});

    bool foundSceneFile = !sceneFilepaths.IsEmpty();
    if (foundSceneFile)
    {
        SceneManager::OpenScene(sceneFilepaths.Front());
    }

    return foundSceneFile;
}

#include "Bang/ProjectManager.h"

#include <string>

#include "Bang/Paths.h"
#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/XMLNode.h"
#include "Bang/XMLParser.h"
#include "Bang/SceneManager.h"
#include "Bang/EngineConfig.h"

#ifdef BANG_EDITOR
#include "Bang/Dialog.h"
#include "Bang/MenuBar.h"
#include "Bang/Explorer.h"
#include "Bang/EditorWindow.h"
#include "Bang/QtProjectManager.h"
#endif

Project *ProjectManager::s_currentProject = nullptr;

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::OpenProject(const Path &projectFilepath)
{
    XMLNode xmlInfo = XMLParser::FromFile(projectFilepath);

    #ifdef BANG_EDITOR
    ProjectManager::CloseCurrentProject();
    #endif

    ProjectManager::s_currentProject = new Project();
    Project *currentProject = ProjectManager::s_currentProject;
    currentProject->Read(xmlInfo);
    currentProject->SetProjectRootFilepath( projectFilepath.GetDirectory() );

    #ifdef BANG_EDITOR
    // Set directory of the explorer to the Assets' root of the new project
    Path projectAssetsDir = currentProject->GetProjectAssetsRootFilepath();
    Explorer::GetInstance()->SetDir( Path(projectAssetsDir) );

    EngineConfig::RegisterInRecentProjectsList(projectFilepath);

    EditorWindow::GetInstance()->RefreshDocksAndWindowTitles();
    #endif

    Paths::SetProjectPath(currentProject->GetProjectDirPath());

    // Open the first found scene
    List<Path> sceneFilepaths = Paths::ProjectAssets().GetFiles(true,
                                   {"*." + Scene::GetFileExtensionStatic()});
    #ifdef BANG_EDITOR
    QtProjectManager::CreateQtProjectFile();
    #endif

    if (!sceneFilepaths.Empty())
    {
        SceneManager::OpenScene(sceneFilepaths.Front());
    }
    else
    {
        #ifdef BANG_EDITOR
        // Create new empty scene
        MenuBar::GetInstance()->CreateNewScene();
        #endif
    }
    //

    return currentProject;
}

#ifdef BANG_EDITOR
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
                             Project::GetFileExtensionStatic());

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

Path ProjectManager::DialogCreateNewProject()
{
    Path dirPath = Dialog::GetOpenDirpath(
                                   "Select the project containing directory");
    if (!dirPath.IsEmpty())
    {
        bool ok;
        String projectName =
                Dialog::GetInputString("Please specify your new project's name",
                                       "Project name:",
                                       "MyBangProject",
                                       &ok);
        if (ok)
        {
            Path projectPath = Path(dirPath).Append(projectName);
            if (!projectPath.IsDir())
            {
                ProjectManager::CreateNewProject(dirPath, projectName);
                return ProjectManager::GetCurrentProject()->
                                       GetProjectFileFilepath();
            }
            else
            {
                Dialog::Error(
                      "Error creating the project",
                      "The directory '" + projectPath + "' existed before.\n" +
                      "Please select another name for your project.");
            }
        }
    }
    return Path::Empty;
}

Path ProjectManager::DialogOpenProject()
{
    Path projectFilepath =
            Dialog::GetOpenFilepath("Select the project file to be opened",
                                    Project::GetFileExtensionStatic(),
                                    Paths::Home());
    return projectFilepath;
}
#endif

Project *ProjectManager::GetCurrentProject()
{
    return ProjectManager::s_currentProject;
}

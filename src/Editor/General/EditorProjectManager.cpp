#include "Bang/EditorProjectManager.h"

#include "Bang/Project.h"
#include "Bang/MenuBar.h"
#include "Bang/Explorer.h"
#include "Bang/Extensions.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/EngineConfig.h"
#include "Bang/QtProjectManager.h"

EditorProjectManager::EditorProjectManager()
{

}

Project *EditorProjectManager::OpenProject(const Path &projectFilepath) const
{
    Project *currentProject = ProjectManager::OpenProject(projectFilepath);

    // Set directory of the explorer to the Assets' root of the new project
    Path projectAssetsDir = currentProject->GetProjectAssetsRootFilepath();
    Explorer::GetInstance()->SetDir( Path(projectAssetsDir) );
    EngineConfig::RegisterInRecentProjectsList(projectFilepath);
    EditorWindow::GetInstance()->RefreshDocksAndWindowTitles();

    QtProjectManager::CreateQtProjectFile();

    return currentProject;
}

bool EditorProjectManager::OpenFirstFoundScene(const Path &projectDirPath) const
{
    bool foundScene = ProjectManager::OpenFirstFoundScene(projectDirPath);
    if (!foundScene)
    {
        // Create new empty scene, if we could not open any
        MenuBar::GetInstance()->CreateNewScene();
    }
    return foundScene;
}

Path EditorProjectManager::DialogCreateNewProject()
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

Path EditorProjectManager::DialogOpenProject()
{
    Path projectFilepath =
            Dialog::GetOpenFilepath("Select the project file to be opened",
                                    Extensions::Get<Project>(),
                                    Paths::Home());
    return projectFilepath;
}


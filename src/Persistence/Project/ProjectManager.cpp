#include "Bang/ProjectManager.h"

#include <string>

#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/XMLNode.h"
#include "Bang/XMLParser.h"
#include "Bang/IO.h"
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

Project* ProjectManager::OpenProject(const String &projectFilepath)
{
    XMLNode *xmlInfo = XMLParser::FromFile(projectFilepath);
    if (!xmlInfo)
    {
        Debug_Error("Could not open project '" << projectFilepath << "'");
        return nullptr;
    }

    #ifdef BANG_EDITOR
    ProjectManager::CloseCurrentProject();
    #endif

    ProjectManager::s_currentProject = new Project();
    ProjectManager::s_currentProject->Read(*xmlInfo);
    String projectDir = IO::GetDir(projectFilepath);
    ProjectManager::s_currentProject->SetProjectRootFilepath(projectDir);

    #ifdef BANG_EDITOR
    // Set directory of the explorer to the Assets' root of the new project
    Explorer::GetInstance()->SetDir(
             ProjectManager::s_currentProject->GetProjectAssetsRootFilepath());

    EngineConfig::RegisterInRecentProjectsList(projectFilepath);

    // Set window title
    String title = "Bang - " +
            ProjectManager::s_currentProject->GetProjectName();
    EditorWindow::GetInstance()->GetMainWindow()->
            setWindowTitle(title.ToQString());
    #endif

    // Set persistence variables
    IO::GetInstance()->c_ProjectRootAbsolute =
            ProjectManager::s_currentProject->GetProjectRootFilepath();
    IO::GetInstance()->c_ProjectAssetsRootAbsolute =
            IO::GetProjectRootAbs() + "/Assets";

    // Open the first found scene
    List<String> sceneFilepaths =
            IO::GetFiles(IO::GetProjectAssetsRootAbs(), true,
                                  {"*." + Scene::GetFileExtensionStatic()});

    #ifdef BANG_EDITOR
    QtProjectManager::CreateQtProjectFile();
    #endif

    if (!sceneFilepaths.Empty())
    {
        SceneManager::LoadSceneInstantly(sceneFilepaths.Front());
    }
    else
    {
        #ifdef BANG_EDITOR
        // Create new empty scene
        MenuBar::GetInstance()->CreateNewScene();
        #endif
    }
    //

    return ProjectManager::s_currentProject;
}

#ifdef BANG_EDITOR
Project* ProjectManager::CreateNewProject(const String &projectContainingDir,
                                          const String &projectName)
{
    String projectDir = projectContainingDir + "/" + projectName;
    if (!IO::ExistsDirectory(projectDir))
    {
        if (!IO::CreateDirectory(projectDir))
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

    String projectFileFilepath = projectDir + "/" +
            projectName + "." + Project::GetFileExtensionStatic();

    ProjectManager::s_currentProject =
            CreateNewProjectFileOnly(projectFileFilepath);
    ProjectManager::s_currentProject->SetProjectRootFilepath(projectDir);

    IO::CreateDirectory(projectDir + "/Assets");

    return ProjectManager::s_currentProject;
}

Project *ProjectManager::CreateNewProjectFileOnly(const String &projectFilepath)
{
    Project *proj = new Project();
    proj->WriteToFile(projectFilepath);
    return proj;
}

void ProjectManager::SaveProject(const Project *project)
{
    ASSERT(project);
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

String ProjectManager::DialogCreateNewProject()
{
    String dirPath = Dialog::GetOpenDirname(
                "Select the project containing directory");
    if (!dirPath.Empty())
    {
        bool ok;
        String projectName =
                Dialog::GetInputString("Please specify your new project's name",
                                       "Project name:",
                                       "MyBangProject",
                                       &ok);
        if (ok)
        {
            String projectPath = dirPath + "/" + projectName;
            if (!IO::ExistsDirectory(projectPath))
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
    return "";
}

String ProjectManager::DialogOpenProject()
{
    String projectFilepath =
            Dialog::GetOpenFilename("Select the project file to be opened",
                                    Project::GetFileExtensionStatic(),
                                    String(QDir::homePath()) );
    return projectFilepath;
}
#endif

Project *ProjectManager::GetCurrentProject()
{
    return ProjectManager::s_currentProject;
}

#include "ProjectManager.h"

#include "Debug.h"
#include "Project.h"
#include "XMLNode.h"
#include "MenuBar.h"
#include "Explorer.h"
#include "XMLParser.h"
#include "FileWriter.h"
#include "Persistence.h"

Project *ProjectManager::currentProject = nullptr;

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::NewProject(const String &projectContainingDir,
                                    const String &projectName)
{
    String projectDir = projectContainingDir + "/" + projectName;
    if (!Persistence::ExistsDirectory(projectDir))
    {
        if (!Persistence::CreateDirectory(projectDir))
        {
            Debug_Error ("Could not create project in directory '" << projectDir << "'.");
            return nullptr;
        }
    }
    else
    {
        // TODO: Add overwrite window warning
        Debug_Warn("Directory '" << projectDir << "' already existed, using it.");
    }

    ProjectManager::CloseCurrentProject();

    ProjectManager::currentProject = new Project();
    ProjectManager::currentProject->SetProjectRootFilepath(projectDir);

    String projectFileFilepath = projectDir + "/" +
            projectName + "." + Project::GetFileExtensionStatic();
    FileWriter::WriteToFile(projectFileFilepath,
                            ProjectManager::currentProject->GetXMLInfoString());

    Persistence::CreateDirectory(projectDir + "/Assets");

    return ProjectManager::currentProject;
}

Project* ProjectManager::OpenProject(const String &projectFilepath)
{
    XMLNode *xmlInfo = XMLParser::FromFile(projectFilepath);
    if (!xmlInfo)
    {
        Debug_Error("Could not open project '" << projectFilepath << "'");
    }

    ProjectManager::CloseCurrentProject();

    ProjectManager::currentProject = new Project();
    ProjectManager::currentProject->ReadXMLInfo(xmlInfo);
    ProjectManager::currentProject->
            SetProjectRootFilepath( Persistence::GetDir(projectFilepath) );

    // Create new scene
    MenuBar::GetInstance()->OnNewScene();

    // Set directory of the explorer to the Assets' root of the new project
    Explorer::GetInstance()->SetDir(
                ProjectManager::currentProject->GetProjectAssetsRootFilepath());

    return ProjectManager::currentProject;
}

void ProjectManager::SaveProject(const Project *project)
{
    NONULL(project);
    bool ok = FileWriter::WriteToFile(project->GetProjectFileFilepath(),
                                      project->GetXMLInfoString());
    if (ok)
    {
        Debug_Status("Project '" << project->GetProjectName() << "' successfully saved.");
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
    if (ProjectManager::currentProject)
    {
        delete currentProject;
        ProjectManager::currentProject = nullptr;
    }

}

Project *ProjectManager::GetCurrentProject()
{
    return ProjectManager::currentProject;
}

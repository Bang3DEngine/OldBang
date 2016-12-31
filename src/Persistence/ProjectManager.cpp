#include "ProjectManager.h"

#include "Debug.h"
#include "Project.h"
#include "XMLNode.h"
#include "XMLParser.h"
#include "Persistence.h"

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::NewProject(const String &projectContainingDir,
                                    const String &projectName)
{
    String projectDir = projectContainingDir + projectName;
    if (Persistence::ExistsDirectory(projectDir))
    {
        Debug_Error ("Can not create project, directory '" << projectDir << "' already exists.");
        return nullptr;
    }

    if (!Persistence::CreateDirectory(projectDir))
    {
        Debug_Error ("Could not create project in directory '" << projectDir << "'.");
        return nullptr;
    }

    Project *project = new Project();
    project->SetProjectRootFilepath(projectDir);

    Persistence::CreateDirectory(projectDir + "/Assets");

    return project;
}

Project* ProjectManager::OpenProject(const String &projectFilepath)
{
    XMLNode *xmlInfo = XMLParser::FromFile(projectFilepath);
    if (!xmlInfo)
    {
        Debug_Error("Could not open project '" << projectFilepath << "'");
    }

    Project *project = new Project();
    project->ReadXMLInfo(xmlInfo);

    return project;
}

void ProjectManager::SaveProject(const Project &project, const String &projectFilepath)
{

}

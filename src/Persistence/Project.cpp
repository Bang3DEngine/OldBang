#include "Project.h"

#include "Persistence.h"

Project::Project()
{
}

Project::~Project()
{
}

const String Project::GetFileExtensionStatic()
{
    return "bproject";
}

const String Project::GetFileExtension()
{
    return Project::GetFileExtensionStatic();
}

void Project::ReadXMLInfo(const XMLNode *xmlInfo)
{

}

void Project::FillXMLInfo(XMLNode *xmlInfo) const
{

}

const String&  Project::GetProjectRootFilepath() const
{
    return m_projectRootFilepath;
}

String Project::GetProjectAssetsRootFilepath() const
{
    return m_projectRootFilepath + "/Assets";
}

String Project::GetProjectName() const
{
    return Persistence::GetFileName(m_projectRootFilepath);
}

void Project::SetProjectRootFilepath(const String &projectDir)
{
    m_projectRootFilepath = projectDir;
}

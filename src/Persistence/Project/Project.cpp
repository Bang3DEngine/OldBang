#include "Project.h"

#include <string>

#include "Time.h"
#include "XMLNode.h"
#include "IO.h"

Project::Project()
{
    m_randomId = std::to_string(Time::GetNow());
}

Project::~Project()
{
}

String Project::GetFileExtensionStatic()
{
    return "bproject";
}

String Project::GetFileExtension()
{
    return Project::GetFileExtensionStatic();
}

void Project::Read(const XMLNode *xmlInfo)
{
    m_randomId = xmlInfo->GetString("RandomID");
}

void Project::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Project");
    xmlInfo->SetString("RandomID", m_randomId);
}

const String&  Project::GetProjectRootFilepath() const
{
    return m_projectRootFilepath;
}

String Project::GetProjectDir() const
{
    return IO::GetDir(m_projectRootFilepath);
}

String Project::GetProjectAssetsRootFilepath() const
{
    return m_projectRootFilepath + "/Assets";
}

String Project::GetProjectFileFilepath() const
{
    return m_projectRootFilepath + "/" +
            GetProjectName() + "." + GetFileExtensionStatic();
}

String Project::GetProjectName() const
{
    return IO::GetFileName(m_projectRootFilepath);
}

String Project::GetProjectRandomId() const
{
    return m_randomId;
}

void Project::SetProjectRootFilepath(const String &projectDir)
{
    m_projectRootFilepath = projectDir;
}

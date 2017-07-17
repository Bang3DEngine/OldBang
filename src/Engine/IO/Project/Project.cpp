#include "Bang/Project.h"

#include <string>

#include "Bang/Paths.h"
#include "Bang/Time.h"
#include "Bang/XMLNode.h"
#include "Bang/Extensions.h"

Project::Project()
{
    m_randomId = std::to_string(Time::GetNow());
}

Project::~Project()
{
}

void Project::Read(const XMLNode &xmlInfo)
{
    m_randomId = xmlInfo.GetString("RandomID");
}

void Project::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Project");
    xmlInfo->SetString("RandomID", m_randomId);
}

const Path&  Project::GetProjectDirPath() const
{
    return m_projectRootFilepath;
}

Path Project::GetProjectDir() const
{
    return m_projectRootFilepath.GetDirectory();
}

Path Project::GetProjectAssetsRootFilepath() const
{
    return Path(m_projectRootFilepath + "/Assets");
}

Path Project::GetProjectFileFilepath() const
{
    return Path(m_projectRootFilepath + "/" +
                GetProjectName() + "." + Extensions::Get<Project>());
}

String Project::GetProjectName() const
{
    return m_projectRootFilepath.GetName();
}

String Project::GetProjectRandomId() const
{
    return m_randomId;
}

void Project::SetProjectRootFilepath(const Path &projectDir)
{
    m_projectRootFilepath = projectDir;
}

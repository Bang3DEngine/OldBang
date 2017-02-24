#include "QtProjectManager.h"

#include "Debug.h"
#include "Project.h"
#include "SystemUtils.h"
#include "Persistence.h"
#include "EditorWindow.h"
#include "ProjectManager.h"

String QtProjectManager::GetQtProjectFilepathDir()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    if (!p_proj) { return ""; }

    String projAbsDir = p_proj->GetProjectRootFilepath();
    return projAbsDir + "/Project";
}

String QtProjectManager::GetQtProjectFilepath()
{
    return QtProjectManager::GetQtProjectFilepathPrefix() + ".creator";
}

String QtProjectManager::GetQtProjectFilepathPrefix()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    if (!p_proj) { return ""; }

    String projName = p_proj->GetProjectName();
    return QtProjectManager::GetQtProjectFilepathDir() + "/" + projName;
}

bool QtProjectManager::IsQtCreatorOpenedCurrentProject()
{
    String projFilepath = QtProjectManager::GetQtProjectFilepath();
    String query = "ps -e";

    String output;
    SystemUtils::System(query, &output);
    return output.Contains("qtcreator");
}

void QtProjectManager::CreateQtProjectFile()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    String projAbsDir = p_proj->GetProjectRootFilepath();
    String projName = p_proj->GetProjectName();
 const String &engineAbsDir = Persistence::GetEngineRootAbs();
    const String &projAssetsDir = projAbsDir + "/Assets";

    List<String> headers = Persistence::GetFiles(projAssetsDir, true, {"h"});
    List<String> engineHeaders =
            Persistence::GetFiles(engineAbsDir, true, {"h"});

    List<String> sources = Persistence::GetFiles(projAssetsDir, true, {"cpp"});
    List<String> engineSources =
            Persistence::GetFiles(engineAbsDir, true, {"cpp"});

    List<String> projIncludePaths =
         Persistence::GetSubDirectories(projAssetsDir, true);
    List<String> engineIncludePaths =
         Persistence::GetSubDirectories(engineAbsDir, true);

    String headersString = String::Join(headers, "\n");
    String engineHeadersString = String::Join(engineHeaders, "\n");
    String sourcesString = String::Join(sources, "\n");
    String engineSourcesString = String::Join(engineSources, "\n");
    String projIncludePathsString = String::Join(projIncludePaths, "\n");
    String engineIncludePathsString = String::Join(engineIncludePaths, "\n");

    Persistence::CreateDirectory(QtProjectManager::GetQtProjectFilepathDir());

    String projFilepathPrefix = QtProjectManager::GetQtProjectFilepathPrefix();

    Persistence::WriteToFile(projFilepathPrefix + ".files",
                                  headersString + "\n" +
                                  sourcesString + "\n" +
                                  engineHeadersString + "\n" +
                                  engineSourcesString);

    Persistence::WriteToFile(projFilepathPrefix + ".includes",
                                  projIncludePathsString + "\n" +
                                  engineIncludePathsString);

    Persistence::WriteToFile(projFilepathPrefix + ".creator", "[General]");
    Persistence::WriteToFile(projFilepathPrefix + ".config", "");
}

void QtProjectManager::OpenBehaviourInQtCreator(const String &behFilepath)
{
    bool alreadyOpened = QtProjectManager::IsQtCreatorOpenedCurrentProject();
    String qtProjFilepath = alreadyOpened ?
                " -client " : QtProjectManager::GetQtProjectFilepath();
    String cmd = "qtcreator " + qtProjFilepath + " " + behFilepath;
    SystemUtils::SystemBackground(cmd);
}

QtProjectManager::QtProjectManager()
{
}

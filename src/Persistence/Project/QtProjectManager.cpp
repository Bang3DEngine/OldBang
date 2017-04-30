#include "Bang/QtProjectManager.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/SystemUtils.h"
#include "Bang/EditorWindow.h"
#include "Bang/ProjectManager.h"

String QtProjectManager::GetQtProjectFilepathDir()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    if (!p_proj) { return ""; }

    String projAbsDir = p_proj->GetProjectRootFilepath().GetAbsolute();
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
    String output;
    SystemUtils::System("ps", {"-eo", "command"}, &output);

    bool alreadyOpened = false;
    List<String> outputLines = output.Split('\n').ToList();
    for (const String &line : outputLines)
    {
        if (line.Contains("qtcreator") && !line.Contains("Bang.pro"))
        {
            alreadyOpened = true;
            break;
        }
    }
    return alreadyOpened;
}

void QtProjectManager::CreateQtProjectFile()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    String projAbsDir = p_proj->GetProjectRootFilepath().GetAbsolute();
    const String &engineAbsDir = IO::GetEngineRootAbs();
    const String &projAssetsDir = projAbsDir + "/Assets";

    List<String> headers = IO::GetFiles(projAssetsDir, true, {"h"});
    List<String> engineHeaders =
            IO::GetFiles(engineAbsDir, true, {"h"});

    List<String> sources = IO::GetFiles(projAssetsDir, true, {"cpp"});
    List<String> engineSources =
            IO::GetFiles(engineAbsDir, true, {"cpp"});

    List<String> projIncludePaths =
         IO::GetSubDirectories(projAssetsDir, true);
    List<String> engineIncludePaths = { IO::GetEngineRootAbs() + "/include" };

    String headersString = String::Join(headers, "\n");
    String engineHeadersString = String::Join(engineHeaders, "\n");
    String sourcesString = String::Join(sources, "\n");
    String engineSourcesString = String::Join(engineSources, "\n");
    String projIncludePathsString = String::Join(projIncludePaths, "\n");
    String engineIncludePathsString = String::Join(engineIncludePaths, "\n");

    IO::CreateDirectory(QtProjectManager::GetQtProjectFilepathDir());

    String projFilepathPrefix = QtProjectManager::GetQtProjectFilepathPrefix();

    IO::WriteToFile(projFilepathPrefix + ".files",
                             headersString + "\n" +
                             sourcesString + "\n" +
                             engineHeadersString + "\n" +
                             engineSourcesString);

    IO::WriteToFile(projFilepathPrefix + ".includes",
                             projIncludePathsString + "\n" +
                             engineIncludePathsString);

    IO::WriteToFile(projFilepathPrefix + ".creator", "[General]");
    IO::WriteToFile(projFilepathPrefix + ".config", "");
}

void QtProjectManager::OpenBehaviourInQtCreator(const String &behFilepath)
{
    bool alreadyOpened = QtProjectManager::IsQtCreatorOpenedCurrentProject();

    List<String> args = {};
    if (alreadyOpened) { args.PushBack("-client"); }
    else { args.PushBack(QtProjectManager::GetQtProjectFilepath()); }
    args.PushBack(behFilepath);
    SystemUtils::SystemBackground("qtcreator", args);
}

QtProjectManager::QtProjectManager()
{
}

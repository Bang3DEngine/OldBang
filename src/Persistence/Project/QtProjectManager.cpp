#include "Bang/QtProjectManager.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/SystemUtils.h"
#include "Bang/EditorWindow.h"
#include "Bang/ProjectManager.h"

Path QtProjectManager::GetQtProjectDir()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    if (!p_proj) { return Path::Empty; }
    return p_proj->GetProjectDirPath().Append("Project");
}

Path QtProjectManager::GetQtProjectFilepath()
{
    return QtProjectManager::GetQtProjectDir().Append(".creator");
}

bool QtProjectManager::IsQtCreatorOpenedCurrentProject()
{
    String output;
    SystemUtils::System("ps", {"-eo", "command"}, &output);

    bool alreadyOpened = false;
    List<String> outputLines = output.Split('\n').To<List>();
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
    Path projectDir = p_proj->GetProjectDirPath();
    const Path &engineDir = Paths::Engine();
    const Path &projAssetsDir = projectDir.Append("Assets");

    List<String> headers =
            projAssetsDir.GetFiles(true, {"h"}).To<List, String>();
    List<String> engineHeaders =
            engineDir.GetFiles(true, {"h"}).To<List, String>();
    List<String> sources =
            projAssetsDir.GetFiles(true, {"cpp"}).To<List, String>();
    List<String> engineSources =
            engineDir.GetFiles(true, {"cpp"}).To<List, String>();
    List<String> projIncPaths =
            projAssetsDir.GetSubDirectories(true).To<List, String>();

    List<Path> engineIncPaths = { Paths::Engine().Append("include") };
    List<String> engineIncPathsStr = engineIncPaths.To<List, String>();

    String headersString            = String::Join(headers,           "\n");
    String engineHeadersString      = String::Join(engineHeaders,     "\n");
    String sourcesString            = String::Join(sources,           "\n");
    String engineSourcesString      = String::Join(engineSources,     "\n");
    String projIncludePathsString   = String::Join(projIncPaths,      "\n");
    String engineIncludePathsString = String::Join(engineIncPathsStr, "\n");

    Path qtProjDir = QtProjectManager::GetQtProjectDir();
    File::CreateDirectory(qtProjDir);
    File::Write(qtProjDir.Append(".files"),
                headersString       + "\n" +
                sourcesString       + "\n" +
                engineHeadersString + "\n" +
                engineSourcesString);

    File::Write(qtProjDir.Append(".includes"),
                projIncludePathsString + "\n" +
                engineIncludePathsString);

    File::Write(qtProjDir.Append(".creator"), "[General]");
    File::Write(qtProjDir.Append(".config"), "");
}

void QtProjectManager::OpenBehaviourInQtCreator(const Path &behFilepath)
{
    bool alreadyOpened = QtProjectManager::IsQtCreatorOpenedCurrentProject();

    List<String> args = {};
    if (alreadyOpened) { args.Add("-client"); }
    //else
    {
        Path qtProjFilepath = QtProjectManager::GetQtProjectFilepath();
        args.Add(qtProjFilepath.GetAbsolute());
    }
    args.Add(behFilepath.GetAbsolute());
    SystemUtils::SystemBackground("qtcreator", args);
}

QtProjectManager::QtProjectManager()
{
}

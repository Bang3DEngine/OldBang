#include "Bang/QtProjectManager.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/SystemUtils.h"
#include "Bang/EditorWindow.h"
#include "Bang/ProjectManager.h"

Path QtProjectManager::GetQtProjectDir()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    if (!p_proj) { return Path::Empty; }
    return p_proj->GetProjectRootFilepath().Append("Project");
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
    Path projectDir = p_proj->GetProjectRootFilepath();
    const Path &engineDir = Path(IO::GetEngineRootAbs());
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

    List<Path> engineIncPaths = { Path(IO::GetEngineRootAbs()).Append("include") };
    List<String> engineIncPathsStr = engineIncPaths.To<List, String>();

    String headersString            = String::Join(headers,           "\n");
    String engineHeadersString      = String::Join(engineHeaders,     "\n");
    String sourcesString            = String::Join(sources,           "\n");
    String engineSourcesString      = String::Join(engineSources,     "\n");
    String projIncludePathsString   = String::Join(projIncPaths,      "\n");
    String engineIncludePathsString = String::Join(engineIncPathsStr, "\n");

    Path qtProjDir = QtProjectManager::GetQtProjectDir();
    IO::CreateDirectory(qtProjDir);
    IO::WriteToFile(qtProjDir.Append(".files"),
                    headersString       + "\n" +
                    sourcesString       + "\n" +
                    engineHeadersString + "\n" +
                    engineSourcesString);

    IO::WriteToFile(qtProjDir.Append(".includes"),
                    projIncludePathsString + "\n" +
                    engineIncludePathsString);

    IO::WriteToFile(qtProjDir.Append(".creator"), "[General]");
    IO::WriteToFile(qtProjDir.Append(".config"), "");
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

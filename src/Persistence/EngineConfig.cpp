#include "Bang/EngineConfig.h"

#include "Bang/FileReader.h"
#include "Bang/IO.h"

EngineConfig::EngineConfig()
{
}

void EngineConfig::CleanOutdatedRecentProjectList()
{
    List<Path> recentProjectsList = EngineConfig::GetRecentProjectsList();
    for (auto it = recentProjectsList.Begin(); it != recentProjectsList.End();)
    {
        Path recentProject = *it;
        if (!recentProject.IsFile() ||                              // Remove non-existing
            recentProjectsList.CountOccurrences(recentProject) > 1) // Remove repeated
        {
            recentProjectsList.Remove(it++);
        }
        else { ++it; }
    }

    WriteListToFile(EngineConfig::GetRecentProjectsFilepath(),
                    recentProjectsList.To<List, String>());
}

void EngineConfig::WriteListToFile(const Path &filepath,
                                   const List<String> &list)
{
    String content = "";
    for (String str : list)
    {
        content += str;
        content += "\n";
    }
    IO::WriteToFile(filepath, content);
}

Path EngineConfig::GetRecentProjectsFilepath()
{
    return Path(IO::GetEngineRootAbs()).Append("config/RecentProjects.cfg");
}

List<Path> EngineConfig::GetRecentProjectsList()
{
    Path recentProjectConfigFilepath = EngineConfig::GetRecentProjectsFilepath();
    String recentProjectsContent = IO::GetFileContents(recentProjectConfigFilepath);
    Array<String> projectsList = recentProjectsContent.Split('\n');
    return projectsList.To<List,Path>();
}

void EngineConfig::RegisterInRecentProjectsList(const Path &projectFilepath)
{
    Path recentProjectConfigFilepath = EngineConfig::GetRecentProjectsFilepath();

    List<Path> recentProjectsList = EngineConfig::GetRecentProjectsList();
    recentProjectsList.Remove(projectFilepath);    // Remove it (to move later)
    recentProjectsList.PushFront(projectFilepath); // Add the first one

    EngineConfig::WriteListToFile(recentProjectConfigFilepath,
                                  recentProjectsList.To<List,String>());
}

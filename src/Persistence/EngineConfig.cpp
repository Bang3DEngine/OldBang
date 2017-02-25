#include "EngineConfig.h"

#include "FileWriter.h"
#include "FileReader.h"
#include "Persistence.h"

EngineConfig::EngineConfig()
{
}

void EngineConfig::CleanOutdatedRecentProjectList()
{
    List<String> recentProjectsList = EngineConfig::GetRecentProjectsList();
    for (auto it = recentProjectsList.Begin();
         it != recentProjectsList.End(); ++it)
    {
        String recentProject = *it;
        if (!Persistence::ExistsFile(recentProject) ||              // Remove non-existing
            recentProjectsList.CountOccurrences(recentProject) > 1) // Remove repeated
        {
            it = recentProjectsList.Remove(it);
            --it;
        }
    }

    WriteListToFile(EngineConfig::GetRecentProjectsFilepath(),
                    recentProjectsList);
}

void EngineConfig::WriteListToFile(const String &filepath,
                                   const List<String> &list)
{
    String content = "";
    for (String str : list)
    {
        content += str;
        content += "\n";
    }

    Persistence::WriteToFile(filepath, content);
}

String EngineConfig::GetRecentProjectsFilepath()
{
    return Persistence::GetEngineRootAbs() + "/config/RecentProjects.cfg";
}

List<String> EngineConfig::GetRecentProjectsList()
{
    String recentProjectConfigFilepath = EngineConfig::GetRecentProjectsFilepath();
    String recentProjectsContent =
            FileReader::GetContents(recentProjectConfigFilepath);

    Array<String> projectsList = recentProjectsContent.Split('\n');

    return projectsList.ToList();
}

void EngineConfig::RegisterInRecentProjectsList(const String &projectFilepath)
{
    String recentProjectConfigFilepath = EngineConfig::GetRecentProjectsFilepath();

    List<String> recentProjectsList = EngineConfig::GetRecentProjectsList();
    recentProjectsList.Remove(projectFilepath);    // Remove it (to move later)
    recentProjectsList.PushFront(projectFilepath); // Add the first one

    EngineConfig::WriteListToFile(recentProjectConfigFilepath,
                                  recentProjectsList);
}

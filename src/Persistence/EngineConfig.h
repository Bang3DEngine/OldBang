#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/String.h"

class EngineConfig
{
private:
    EngineConfig();

    static void WriteListToFile(const Path &filepath,
                                const List<String> &list);
    static Path GetRecentProjectsFilepath();

public:

    static void CleanOutdatedRecentProjectList();
    static List<Path> GetRecentProjectsList();
    static void RegisterInRecentProjectsList(const Path &projectFilepath);
};

#endif // ENGINECONFIG_H

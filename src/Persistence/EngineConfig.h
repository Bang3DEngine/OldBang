#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "List.h"
#include "String.h"

class EngineConfig
{
private:
    EngineConfig();

    static void WriteListToFile(const String &filepath,
                                const List<String> &list);
    static String GetRecentProjectsFilepath();

public:

    static void CleanOutdatedRecentProjectList();
    static List<String> GetRecentProjectsList();
    static void RegisterInRecentProjectsList(const String &projectFilepath);
};

#endif // ENGINECONFIG_H

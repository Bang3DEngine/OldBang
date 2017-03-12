#include "BehaviourMergeObjectsRunnable.h"

#include "Time.h"
#include "List.h"
#include "Debug.h"
#include "XMLNode.h"
#include "Project.h"
#include "SystemUtils.h"
#include "IO.h"
#include "ProjectManager.h"
#include "BehaviourManager.h"

BehaviourMergeObjectsRunnable::BehaviourMergeObjectsRunnable(bool forGame)
{
    m_forGame = forGame;
}

void BehaviourMergeObjectsRunnable::run()
{
    Merge();
}

void BehaviourMergeObjectsRunnable::Merge()
{
    List<String> behaviourObjects =
            BehaviourManager::GetBehavioursObjectsFilepathsList();
    String libOutputFilepath =
            BehaviourManager::GetCurrentLibsDir() + "/Behaviours.so.";
    String version = "";
    if (!m_forGame)
    {
        std::to_string(Time::GetNow());
    }
    else
    {
        String gameProjectDir = IO::GetDir(libOutputFilepath);
        gameProjectDir = IO::GetDirUp(gameProjectDir);
        String gameProjectFilepath = gameProjectDir + "/Game.bproject";

        Project gameProject;
        gameProject.ReadFromFile(gameProjectFilepath);
        version = gameProject.GetProjectRandomId();
    }
    libOutputFilepath += version + ".1.1";

    typedef SystemUtils::CompilationFlags CLFlags;
    bool successCompiling; String output;
    SystemUtils::Compile(behaviourObjects, libOutputFilepath,
                         CLFlags::AddEngineObjectFiles  |
                         CLFlags::ProduceSharedLib |
                         (m_forGame ? CLFlags::ForGame : CLFlags::None),
                         &successCompiling, &output);
    if (successCompiling)
    {
        emit NotifySuccessMerging(libOutputFilepath.ToQString(),
                                  output.ToQString());
    }
    else { emit NotifyFailedMerging(output.ToQString()); }
}


#include "BehaviourMergeObjectsRunnable.h"

#include "Time.h"
#include "List.h"
#include "Debug.h"
#include "XMLNode.h"
#include "Project.h"
#include "SystemUtils.h"
#include "Persistence.h"
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
    String version = std::to_string(Time::GetNow());
    if (m_forGame)
    {
        String gameProjectDir = Persistence::GetDir(libOutputFilepath);
        gameProjectDir = Persistence::GetDirUp(gameProjectDir);
        String gameProjectFilepath = gameProjectDir + "/Game.bproject";

        String gameProjectContents =
                Persistence::GetFileContents(gameProjectFilepath);
        Project *gameProject = new Project();
        gameProject->ReadXMLInfoFromString(gameProjectContents);
        version = gameProject->GetProjectRandomId();
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


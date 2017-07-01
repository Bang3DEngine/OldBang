#include "Bang/BehaviourMergeObjectsRunnable.h"

#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
#include "Bang/Project.h"
#include "Bang/BangCompiler.h"
#include "Bang/ProjectManager.h"
#include "Bang/BehaviourManager.h"

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
    List<Path> behaviourObjects =
            BehaviourManager::GetBehavioursObjectsFilepathsList();
    Path libOutputFilepath(
            BehaviourManager::GetCurrentLibsDir() + "/Behaviours.so.");
    String version = "";
    if (!m_forGame)
    {
        version = String::ToString( Time::GetNow() );
    }
    else
    {
        Path gameProjectDir = libOutputFilepath.GetDirectory().GetDirectory();
        Path gameProjectFilepath(gameProjectDir + "/Game.bproject");

        Project gameProject;
        gameProject.ReadFromFile(gameProjectFilepath);
        version = gameProject.GetProjectRandomId();
    }
    libOutputFilepath = Path(libOutputFilepath.GetAbsolute() + version + ".1.1");

    Compiler::Result compileResult;
    compileResult = BangCompiler::MergeBehaviourLibraries(behaviourObjects,
                                                          libOutputFilepath,
                                                          m_forGame);
    if (compileResult.success)
    {
        emit NotifySuccessMerging(libOutputFilepath.GetAbsolute().ToQString(),
                                  !m_forGame,
                                  compileResult.output.ToQString());
    }
    else { emit NotifyFailedMerging(compileResult.output.ToQString()); }
}


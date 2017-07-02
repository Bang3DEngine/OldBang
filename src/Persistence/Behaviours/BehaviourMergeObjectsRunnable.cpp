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

BehaviourMergeObjectsRunnable::BehaviourMergeObjectsRunnable(bool forGame,
                                                             const Path& outputDir)
{
    m_forGame = forGame;
    m_outputDir = outputDir;
}

void BehaviourMergeObjectsRunnable::run()
{
    List<Path> behaviourObjects = m_outputDir.GetFiles(true, {"o"});
    Path libOutputFilepath = m_outputDir.Append("Behaviours")
                                        .AppendExtension("so");
    String version = "";
    if (!m_forGame)
    {
        version = String::ToString( Time::GetNow() );
    }
    else
    {
        Path gameProjectDir = libOutputFilepath.GetDirectory().GetDirectory();
        Path gameProjectFilepath = gameProjectDir.Append("Game")
                                                 .AppendExtension("bproject");

        Project gameProject;
        gameProject.ReadFromFile(gameProjectFilepath);
        version = gameProject.GetProjectRandomId();
    }
    libOutputFilepath = libOutputFilepath.AppendExtension(version + ".1.1");

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


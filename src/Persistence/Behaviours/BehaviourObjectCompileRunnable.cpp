#include "Bang/BehaviourObjectCompileRunnable.h"

#include "Bang/Debug.h"
#include "Bang/SystemUtils.h"
#include "Bang/BehaviourManager.h"

BehaviourObjectCompileRunnable::BehaviourObjectCompileRunnable(
        const Path &behaviourFilepath, bool forGame)
{
    m_forGame = forGame;
    m_behaviourPath = behaviourFilepath;
}

void BehaviourObjectCompileRunnable::run()
{
    CompileBehaviourObject();
}

void BehaviourObjectCompileRunnable::CompileBehaviourObject()
{
    String behaviourName = m_behaviourPath.GetName();
    Path objFilepath = Path(BehaviourManager::GetCurrentLibsDir())
                            .Append(behaviourName).AppendExtension("o");
    IO::Remove(objFilepath);

    typedef SystemUtils::CompilationFlag CLFlag;

    String output = "";
    bool successCompiling = false;
    List<Path> sources = {m_behaviourPath};
    SystemUtils::Compile(sources,
                         objFilepath,
                         (m_forGame ? CLFlag::ForGame : CLFlag::None) |
                           CLFlag::AddAssetsIncludeDirs,
                         &successCompiling,
                         &output);
    if (successCompiling)
    {
        emit NotifySuccessCompiling(m_behaviourPath.GetAbsolute().ToQString(),
                                    output.ToQString());
    }
    else
    {
        emit NotifyFailedCompiling(m_behaviourPath.GetAbsolute().ToQString(),
                                   output.ToQString());
    }
}

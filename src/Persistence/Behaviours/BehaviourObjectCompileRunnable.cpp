#include "Bang/BehaviourObjectCompileRunnable.h"

#include "Bang/Debug.h"
#include "Bang/SystemUtils.h"
#include "Bang/BehaviourManager.h"

BehaviourObjectCompileRunnable::BehaviourObjectCompileRunnable(
        const String &behaviourFilepath, bool forGame)
{
    m_forGame = forGame;
    m_behaviourFilepath = behaviourFilepath;
}

void BehaviourObjectCompileRunnable::run()
{
    CompileBehaviourObject();
}

void BehaviourObjectCompileRunnable::CompileBehaviourObject()
{
    String behaviourName = IO::GetFileName(m_behaviourFilepath);
    String objFilepath =
            BehaviourManager::GetCurrentLibsDir() + "/" + behaviourName + ".o";

    IO::Remove(objFilepath);

    typedef SystemUtils::CompilationFlags CLFlags;

    String output = "";
    bool successCompiling = false;
    List<String> sources = {m_behaviourFilepath};
    SystemUtils::Compile(sources, objFilepath,
                         (m_forGame ? CLFlags::ForGame : CLFlags::None) |
                              CLFlags::AddAssetsIncludeDirs,
                         &successCompiling, &output);
    if (successCompiling)
    {
        emit NotifySuccessCompiling(m_behaviourFilepath.ToQString(),
                                    output.ToQString());
    }
    else
    {
        emit NotifyFailedCompiling(m_behaviourFilepath.ToQString(),
                                   output.ToQString());
    }
}

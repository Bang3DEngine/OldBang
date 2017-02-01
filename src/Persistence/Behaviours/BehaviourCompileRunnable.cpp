#include "BehaviourCompileRunnable.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourCompileRunnable::
    BehaviourCompileRunnable(const String &behaviourRelativeFilepath)
{
    m_behaviourRelativeFilepath = behaviourRelativeFilepath;
}

void BehaviourCompileRunnable::Compile()
{
    // Compile....
    String soFilepath =
            SystemUtils::CompileToSharedObject(m_behaviourRelativeFilepath);

    BehaviourManager *bm = BehaviourManager::GetInstance();

    if (!soFilepath.Empty())
    {
        // Notify BehaviourManager
        bm->OnBehaviourFinishedCompiling(m_behaviourRelativeFilepath, soFilepath);
    }
    else
    {
        // If it fails, notify the failure to the BehaviourManager, so it doesnt
        // compile until the behaviour file hash changes
        bm->OnBehaviourFailedCompiling(m_behaviourRelativeFilepath);
    }
}

void BehaviourCompileRunnable::run()
{
    Compile();
}

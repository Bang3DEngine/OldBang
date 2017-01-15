#include "BehaviourManagerCompileThread.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourManagerCompileThread::
    BehaviourManagerCompileThread(const String &behaviourRelativeFilepath)
{
    m_behaviourRelativeFilepath = behaviourRelativeFilepath;

    BehaviourManager *bm = BehaviourManager::GetInstance();
}

void BehaviourManagerCompileThread::run()
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

    deleteLater(); // Automatic thread delete
}

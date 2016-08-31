#include "BehaviourManagerCompileThread.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourManagerCompileThread::
    BehaviourManagerCompileThread(const String &behaviourRelativeFilepath)
{
    m_behaviourRelativeFilepath = behaviourRelativeFilepath;
}

void BehaviourManagerCompileThread::run()
{
    // Compile....
    String soFilepath =
            SystemUtils::CompileToSharedObject(m_behaviourRelativeFilepath);

    // Notify BehaviourManager
    BehaviourManager::OnBehaviourFinishedCompiling(m_behaviourRelativeFilepath,
                                                   soFilepath);

    deleteLater(); // Automatic thread delete
}

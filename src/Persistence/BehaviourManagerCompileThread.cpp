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
    Debug_Status("Compiling " << m_behaviourRelativeFilepath);
    // Compile....
    String soFilepath =
            SystemUtils::CompileToSharedObject(m_behaviourRelativeFilepath);

    // Notify BehaviourManager
    BehaviourManager::OnBehaviourFinishedCompiling(m_behaviourRelativeFilepath,
                                                   soFilepath);

    Debug_Status(m_behaviourRelativeFilepath << " compiled.");

    deleteLater(); // Automatic thread delete
}

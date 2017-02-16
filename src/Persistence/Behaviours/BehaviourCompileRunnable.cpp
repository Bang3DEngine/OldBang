#include "BehaviourCompileRunnable.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourCompileRunnable::BehaviourCompileRunnable()
{
}

BehaviourCompileRunnable::
    BehaviourCompileRunnable(const String &behaviourFilepath)
{
    m_behaviourFilepath = behaviourFilepath;

    BehaviourManager *bm = BehaviourManager::GetInstance();
    connect(this, SIGNAL(NotifySuccessCompiling(QString,QString,QString)),
            bm, SLOT(OnBehaviourSuccessCompiling(QString,QString,QString)));
    connect(this, SIGNAL(NotifyFailedCompiling(QString,QString)),
            bm, SLOT(OnBehaviourFailedCompiling(QString,QString)));
}

void BehaviourCompileRunnable::Compile()
{
    // Compile....
    String warnMessage = "";
    String errorMessage = "";
    String libraryFilepath =
            SystemUtils::CompileToSharedObject(
                m_behaviourFilepath, true, &warnMessage, &errorMessage);

    if (!libraryFilepath.Empty())
    {
        // Notify BehaviourManager
        emit NotifySuccessCompiling(m_behaviourFilepath.ToQString(),
                                     libraryFilepath.ToQString(),
                                     warnMessage.ToQString());
    }
    else
    {
        // If it fails, notify the failure to the BehaviourManager, so it doesnt
        // compile until the behaviour file hash changes
        emit NotifyFailedCompiling(m_behaviourFilepath.ToQString(),
                                   errorMessage.ToQString());
    }
}

void BehaviourCompileRunnable::run()
{
    Compile();
}

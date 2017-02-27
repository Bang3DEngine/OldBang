#include "BehavioursLibCompileRunnable.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehavioursLibCompileRunnable::BehavioursLibCompileRunnable()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    connect(this, SIGNAL(NotifySuccessCompiling(QString,QString)),
            bm, SLOT(OnBehaviourSuccessCompiling(QString,QString)));
    connect(this, SIGNAL(NotifyFailedCompiling(QString)),
            bm, SLOT(OnBehaviourFailedCompiling(QString)));
}

void BehavioursLibCompileRunnable::run()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->CompileBehavioursLibrary();
}

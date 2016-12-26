#ifndef BEHAVIOURMANAGERCOMPILETHREAD_H
#define BEHAVIOURMANAGERCOMPILETHREAD_H

#include <QThread>

#include "String.h"

class BehaviourManager;

class BehaviourManagerCompileThread : public QThread
{
protected:
    String m_behaviourRelativeFilepath = "";
    void run() override;

public:
    BehaviourManagerCompileThread(const String &behaviourRelativeFilepath);
};

#endif // BEHAVIOURMANAGERCOMPILETHREAD_H

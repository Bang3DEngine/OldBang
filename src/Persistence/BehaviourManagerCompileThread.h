#ifndef BEHAVIOURMANAGERCOMPILETHREAD_H
#define BEHAVIOURMANAGERCOMPILETHREAD_H

#include <QThread>

#include "String.h"

class BehaviourManager;

class BehaviourManagerCompileThread : public QThread
{

public:
    BehaviourManagerCompileThread(const String &behaviourRelativeFilepath);

protected:
    String m_behaviourRelativeFilepath = "";
    void run() override;
};

#endif // BEHAVIOURMANAGERCOMPILETHREAD_H

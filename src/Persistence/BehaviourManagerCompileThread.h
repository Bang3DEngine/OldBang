#ifndef BEHAVIOURMANAGERCOMPILETHREAD_H
#define BEHAVIOURMANAGERCOMPILETHREAD_H

#include <map>
#include <QThread>
#include <QLibrary>
#include <functional>

#include "Bang.h"
#include "String.h"

class BehaviourManager;

class BehaviourManagerCompileThread// : public QThread
{
protected:
    String m_behaviourRelativeFilepath = "";
    //void run() override;
   public: void start();

public:
    BehaviourManagerCompileThread(const String &behaviourRelativeFilepath);
};

#endif // BEHAVIOURMANAGERCOMPILETHREAD_H

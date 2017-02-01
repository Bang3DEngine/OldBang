#ifndef BEHAVIOURCOMPILERUNNABLE_H
#define BEHAVIOURCOMPILERUNNABLE_H

#include <QRunnable>

#include "String.h"

class BehaviourManager;

class BehaviourCompileRunnable : public QRunnable
{

public:
    BehaviourCompileRunnable(const String &behaviourRelativeFilepath);
    void Compile();

protected:
    String m_behaviourRelativeFilepath = "";
    void run() override;
};

#endif // BEHAVIOURCOMPILERUNNABLE_H

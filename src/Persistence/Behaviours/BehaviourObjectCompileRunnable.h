#ifndef BEHAVIOUROBJECTCOMPILERUNNABLE_H
#define BEHAVIOUROBJECTCOMPILERUNNABLE_H

#include <QObject>
#include <QRunnable>

#include "String.h"

class BehaviourManager;

class BehaviourObjectCompileRunnable : public QObject,
                                       public QRunnable
{
    Q_OBJECT

public:
    BehaviourObjectCompileRunnable(const String &behaviourFilepath);
    void CompileBehavioursLibrary();

protected:
    void run() override;

signals:
    void NotifySuccessCompiling(const QString &behavioursLibFilepath,
                                const QString &warnMessage);
    void NotifyFailedCompiling(const QString &behavioursLibFilepath,
                               const QString &errorMessage);

private:
    String m_behaviourFilepath = "";
    void CompileBehaviourObject();
};

#endif // BEHAVIOUROBJECTCOMPILERUNNABLE_H

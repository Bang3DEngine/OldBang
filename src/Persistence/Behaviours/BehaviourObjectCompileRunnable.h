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
    BehaviourObjectCompileRunnable(const String &behaviourFilepath,
                                   bool forGame);

protected:
    void run() override;

signals:
    void NotifySuccessCompiling(const QString &behavioursLibFilepath,
                                const QString &warnMessage);
    void NotifyFailedCompiling(const QString &behavioursLibFilepath,
                               const QString &errorMessage);

private:
    bool m_forGame = false;
    String m_behaviourFilepath = "";

    void CompileBehaviourObject();
};

#endif // BEHAVIOUROBJECTCOMPILERUNNABLE_H

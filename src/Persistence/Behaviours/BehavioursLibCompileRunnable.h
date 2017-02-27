#ifndef BEHAVIOURSCOMPILERUNNABLE_H
#define BEHAVIOURSCOMPILERUNNABLE_H

#include <QObject>
#include <QRunnable>

#include "String.h"

class BehaviourManager;

class BehavioursLibCompileRunnable : public QObject,
                                     public QRunnable
{
    Q_OBJECT

public:
    BehavioursLibCompileRunnable();
    void CompileBehavioursLibrary();

protected:
    void run() override;

signals:
    void NotifySuccessCompiling(const QString &behavioursLibFilepath,
                                const QString &warnMessage);
    void NotifyFailedCompiling(const QString &errorMessage);
};

#endif // BEHAVIOURSCOMPILERUNNABLE_H

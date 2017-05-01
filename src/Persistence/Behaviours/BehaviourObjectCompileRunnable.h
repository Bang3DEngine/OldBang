#ifndef BEHAVIOUROBJECTCOMPILERUNNABLE_H
#define BEHAVIOUROBJECTCOMPILERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/String.h"

class BehaviourManager;

class BehaviourObjectCompileRunnable : public QObject,
                                       public QRunnable
{
    Q_OBJECT

public:
    BehaviourObjectCompileRunnable(const Path &behaviourFilepath,
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
    Path m_behaviourPath;

    void CompileBehaviourObject();
};

#endif // BEHAVIOUROBJECTCOMPILERUNNABLE_H

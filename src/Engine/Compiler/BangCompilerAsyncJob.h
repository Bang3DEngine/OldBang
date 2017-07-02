#ifndef BANGCOMPILERASYNCJOB_H
#define BANGCOMPILERASYNCJOB_H

#include <QObject>
#include <QRunnable>
#include "Bang/WinUndef.h"

#include "Bang/BangCompiler.h"

class BangCompilerAsyncJob : public QObject,
                             public QRunnable
{
    Q_OBJECT

public:
    BangCompilerAsyncJob(const BangCompiler::Job &compileJob,
                         QObject *resultListener,
                         QSlot slot);

    virtual ~BangCompilerAsyncJob();

protected:
    void run() override;

signals:
    void SignalCompileFinished(const BangCompiler::Result& result);

private:
    BangCompiler::Job m_compileJob;
    QObject *m_resultListener;
    QSlot m_slot;
};

#endif // BANGCOMPILERASYNCJOB_H

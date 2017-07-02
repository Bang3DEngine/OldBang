#include "BangCompilerAsyncJob.h"

#include "Bang/Debug.h"

BangCompilerAsyncJob::BangCompilerAsyncJob(
                                   const BangCompiler::Job &compileJob,
                                   QObject *resultListener,
                                   QSlot slot)
{
    m_compileJob = compileJob;
    m_resultListener = resultListener;
    m_slot = slot;

    QObject::connect(this,
                     SIGNAL(SignalCompileFinished(BangCompiler::Result)),
                     resultListener, slot);
}

BangCompilerAsyncJob::~BangCompilerAsyncJob()
{
    QObject::disconnect(this,
                        SIGNAL(SignalCompileFinished(BangCompiler::Result)),
                        m_resultListener, m_slot);
}

void BangCompilerAsyncJob::run()
{
    Debug_Log("BangCompilerAsyncJob " << m_compileJob.outputFile);
    BangCompiler::Result result = BangCompiler::Compile(m_compileJob);
    emit SignalCompileFinished(result);
}

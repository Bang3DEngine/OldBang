#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Bang/List.h"
#include "Bang/Thread.h"

class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();

    bool TryStart(ThreadRunnable *runnable);

    void SetMaxThreadCount(int maxThreadCount);
    int GetMaxThreadCount() const;

private:
    List<Thread*> m_threadList;

    int m_maxThreadCount = 32;
};

#endif // THREADPOOL_H

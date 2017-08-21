#include "Bang/ThreadPool.h"

ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
    while (!m_threadList.IsEmpty())
    {
        Thread *thread = m_threadList.PopBack();
        delete thread;
    }
}

bool ThreadPool::TryStart(ThreadRunnable *runnable)
{
    auto it = m_threadList.Begin();
    while (it != m_threadList.End())
    {
        Thread *thread = *it;
        if (thread->HasFinished())
        {
            delete thread;
            it = m_threadList.Remove(it);
        }
        else { ++it; }
    }

    if (m_threadList.Size() >= m_maxThreadCount) { return false; }

    Thread *thread = new Thread(runnable,
                                "BANG_PooledThread" + m_threadList.Size());
    thread->Start();
    m_threadList.PushBack(thread);
}

void ThreadPool::SetMaxThreadCount(int maxThreadCount)
{
    m_maxThreadCount = maxThreadCount;
}

int ThreadPool::GetMaxThreadCount() const
{
    return m_maxThreadCount;
}

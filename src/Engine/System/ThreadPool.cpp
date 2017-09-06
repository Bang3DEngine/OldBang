#include "Bang/ThreadPool.h"

USING_NAMESPACE_BANG

ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
    while (!m_threadList.IsEmpty())
    {
        Thread *thread = m_threadList.Back();
        m_threadList.PopBack();
        thread->Join();
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
            thread->Join();
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
    return true;
}

void ThreadPool::SetMaxThreadCount(int maxThreadCount)
{
    m_maxThreadCount = maxThreadCount;
}

int ThreadPool::GetMaxThreadCount() const
{
    return m_maxThreadCount;
}



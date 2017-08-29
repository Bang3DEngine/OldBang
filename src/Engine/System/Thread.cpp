#include "Bang/Thread.h"

#include "Bang/Debug.h"

int ThreadFunc(ThreadRunnable *runnable, Thread *thread);

Thread::Thread()
{
}

Thread::Thread(ThreadRunnable *runnable) : Thread()
{
    SetRunnable(runnable);
}

Thread::Thread(ThreadRunnable *runnable,
               const String &threadName) : Thread(runnable)
{
    m_threadName = threadName;
}

Thread::~Thread()
{
}

void Thread::Start()
{
    if (p_runnable)
    {
        m_thread = std::thread(ThreadFunc, p_runnable, this);
    }
    else { Debug_Error("Trying to start Thread with no ThreadRunnable set."); }
}

void Thread::Join()
{
    m_thread.join();
}

bool Thread::HasFinished() const
{
    return m_hasFinished;
}

void Thread::SetName(const String &threadName) { m_threadName = threadName; }
void Thread::SetRunnable(ThreadRunnable *runnable) { p_runnable = runnable; }

const String &Thread::GetName() const { return m_threadName; }
ThreadRunnable *Thread::GetRunnable() const { return p_runnable; }

void Thread::SleepCurrentThread(float seconds)
{
    int millis = SCAST<int>(seconds * 1000);
    std::this_thread::sleep_for( std::chrono::milliseconds(millis) );
}

String Thread::GetCurrentThreadId()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return String(ss.str());
}

int ThreadFunc(ThreadRunnable *runnable, Thread *thread)
{
    if (runnable)
    {
        runnable->Run();
    }
    if (runnable->IsAutoDelete()) { delete runnable; }

    thread->m_hasFinished = true;

    return 0;
}



ThreadRunnable::ThreadRunnable()
{
}

ThreadRunnable::~ThreadRunnable()
{
}

void ThreadRunnable::SetAutoDelete(bool autoDelete)
{
    m_autoDelete = autoDelete;
}

bool ThreadRunnable::IsAutoDelete() const
{
    return m_autoDelete;
}

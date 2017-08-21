#ifndef THREAD_H
#define THREAD_H

#include <thread>

#include "Bang/String.h"

FORWARD class ThreadRunnable;

class Thread
{
public:
    Thread();
    Thread(ThreadRunnable *runnable);
    Thread(ThreadRunnable *runnable, const String &threadName);
    virtual ~Thread();

    void Start();
    void Join();
    bool HasFinished() const;

    void SetName(const String &threadName);
    void SetRunnable(ThreadRunnable *runnable,
                     bool destroyWhenFinished = true);

    const String &GetName() const;
    ThreadRunnable *GetRunnable() const;

    static void SleepCurrentThread(float seconds);
    static String GetCurrentThreadId();

private:
    std::thread m_thread;
    String m_threadName = "BANG_Thread";
    ThreadRunnable *p_runnable = nullptr;

    bool m_hasFinished = false;
    bool m_destroyRunnable = true;

    friend int ThreadFunc(ThreadRunnable *runnable, Thread *thread);
};


class ThreadRunnable
{
public:
    ThreadRunnable();
    virtual ~ThreadRunnable();

    void SetAutoDelete(bool autoDelete);
    bool GetAutoDelete() const;

    virtual void Run() = 0;

private:
    bool m_autoDelete = true;
};


#endif // THREAD_H

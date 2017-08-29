#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>

#include "Bang.h"

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock();
    void UnLock();

private:
    std::mutex m_mutex;
};

#endif // MUTEX_H

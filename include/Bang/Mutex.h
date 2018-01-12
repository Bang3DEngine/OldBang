#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock();
    bool TryLock();
    void UnLock();

private:
    std::mutex m_mutex;
};

NAMESPACE_BANG_END

#endif // MUTEX_H

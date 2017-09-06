#ifndef MUTEXLOCKER_H
#define MUTEXLOCKER_H

#include "Bang/Mutex.h"

NAMESPACE_BANG_BEGIN

class MutexLocker
{
public:
    MutexLocker(Mutex *mutex);
    ~MutexLocker();

private:
    Mutex *p_mutex;
};

NAMESPACE_BANG_END

#endif // MUTEXLOCKER_H

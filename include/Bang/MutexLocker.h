#ifndef MUTEXLOCKER_H
#define MUTEXLOCKER_H

#include "Bang/Mutex.h"

class MutexLocker
{
public:
    MutexLocker(Mutex *mutex);
    ~MutexLocker();

private:
    Mutex *p_mutex;
};

#endif // MUTEXLOCKER_H

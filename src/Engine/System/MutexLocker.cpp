#include "Bang/MutexLocker.h"

MutexLocker::MutexLocker(Mutex *mutex)
{
    p_mutex = mutex;
    p_mutex->Lock();
}

MutexLocker::~MutexLocker()
{
    p_mutex->UnLock();
}

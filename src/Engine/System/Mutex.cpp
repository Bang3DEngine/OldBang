#include "Bang/Mutex.h"


USING_NAMESPACE_BANG

Mutex::Mutex()
{
}

Mutex::~Mutex()
{
}

void Mutex::Lock()
{
    m_mutex.lock();
}

bool Mutex::TryLock()
{
    return m_mutex.try_lock();
}

void Mutex::UnLock()
{
    m_mutex.unlock();
}

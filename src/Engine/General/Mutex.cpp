#include "Bang/Mutex.h"

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

void Mutex::UnLock()
{
    m_mutex.unlock();
}

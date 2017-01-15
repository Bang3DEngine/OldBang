#include "SingletonManager.h"

#include "Debug.h"
#include "Persistence.h"

#ifdef BANG_EDITOR
#include "ShortcutManager.h"
#endif

SingletonManager *SingletonManager::s_mainBinarySM = nullptr;

void SingletonManager::SetWindowSingleton(IWindow *mainBinaryWindow)
{
    m_mainBinaryWindow = mainBinaryWindow;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryTime)
{
    m_mainBinaryTime = mainBinaryTime;
}

void SingletonManager::SetInputSingleton(Input *mainBinaryInput)
{
    m_mainBinaryInput = mainBinaryInput;
}

void SingletonManager::SetPersistenceSingleton(Persistence *mainBinaryPersistence)
{
    m_mainBinaryPersistence = mainBinaryPersistence;
}

#ifdef BANG_EDITOR
void SingletonManager::SetShortcutManagerSingleton(ShortcutManager *mainBinaryShortcutManager)
{
    m_mainBinaryShortcutManager = mainBinaryShortcutManager;
}
#endif




IWindow* SingletonManager::GetWindowSingleton()
{
    return m_mainBinaryWindow;
}

Time* SingletonManager::GetTimeSingleton()
{
    return m_mainBinaryTime;
}

Input *SingletonManager::GetInputSingleton()
{
    return m_mainBinaryInput;
}

Persistence *SingletonManager::GetPersistenceSingleton()
{
    return m_mainBinaryPersistence;
}

#ifdef BANG_EDITOR
ShortcutManager *SingletonManager::GetShortcutManagerSingleton()
{
    return m_mainBinaryShortcutManager;
}
#endif

void SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM)
{
    SingletonManager::s_mainBinarySM = mainBinarySM;
}


void SingletonManager::InitSingletonManagerFromMainBinary()
{
    SingletonManager::s_mainBinarySM = new SingletonManager();
}

SingletonManager* SingletonManager::GetInstance()
{
    return SingletonManager::s_mainBinarySM;
}

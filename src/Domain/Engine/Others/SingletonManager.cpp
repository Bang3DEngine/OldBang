#include "SingletonManager.h"

#include "Debug.h"
#include "ShortcutManager.h"

SingletonManager *SingletonManager::s_mainBinarySM = nullptr;

void SingletonManager::SetWindowMainSingleton(IWindow *mainBinaryWindowMain)
{
    this->m_mainBinaryWindowMain = mainBinaryWindowMain;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryTime)
{
    this->m_mainBinaryTime = mainBinaryTime;
}

void SingletonManager::SetInputSingleton(Input *mainBinaryInput)
{
    this->m_mainBinaryInput = mainBinaryInput;
}

void SingletonManager::SetShortcutManagerSingleton(ShortcutManager *mainBinaryShortcutManager)
{
    this->m_mainBinaryShortcutManager = mainBinaryShortcutManager;
}





IWindow* SingletonManager::GetWindowSingleton()
{
    return m_mainBinaryWindowMain;
}

Time* SingletonManager::GetTimeSingleton()
{
    return m_mainBinaryTime;
}

Input *SingletonManager::GetInputSingleton()
{
    return m_mainBinaryInput;
}

ShortcutManager *SingletonManager::GetShortcutManagerSingleton()
{
    return m_mainBinaryShortcutManager;
}

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

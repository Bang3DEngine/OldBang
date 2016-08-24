#include "SingletonManager.h"

#include "Debug.h"

#ifdef BANG_EDITOR
#include "ShortcutManager.h"
#endif

SingletonManager *SingletonManager::s_mainBinarySM = nullptr;

void SingletonManager::SetEditorWindowSingleton(IWindow *mainBinaryEditorWindow)
{
    this->m_mainBinaryEditorWindow = mainBinaryEditorWindow;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryTime)
{
    this->m_mainBinaryTime = mainBinaryTime;
}

void SingletonManager::SetInputSingleton(Input *mainBinaryInput)
{
    this->m_mainBinaryInput = mainBinaryInput;
}

#ifdef BANG_EDITOR
void SingletonManager::SetShortcutManagerSingleton(ShortcutManager *mainBinaryShortcutManager)
{
    this->m_mainBinaryShortcutManager = mainBinaryShortcutManager;
}
#endif




IWindow* SingletonManager::GetWindowSingleton()
{
    return m_mainBinaryEditorWindow;
}

Time* SingletonManager::GetTimeSingleton()
{
    return m_mainBinaryTime;
}

Input *SingletonManager::GetInputSingleton()
{
    return m_mainBinaryInput;
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

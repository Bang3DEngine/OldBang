#include "SingletonManager.h"

#include "Debug.h"

#ifdef BANG_EDITOR
#include "ShortcutManager.h"
#endif

SingletonManager *SingletonManager::s_mainBinarySM = nullptr;

void SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(
        SingletonManager *mainBinarySM)
{
    SingletonManager::s_mainBinarySM = mainBinarySM;
}


void SingletonManager::InitSingletonManagerFromMainBinary()
{
    SingletonManager::s_mainBinarySM = new SingletonManager();
}

SingletonManager::SingletonManager()
{
}

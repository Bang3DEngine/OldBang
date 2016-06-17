#include "SingletonManager.h"

SingletonManager *SingletonManager::mainBinarySM = nullptr;


void SingletonManager::SetWindowMainSingleton(WindowMain *mainBinaryWM)
{
    mainBinaryWindowMain = mainBinaryWM;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryT)
{
    mainBinaryTime = mainBinaryT;
}




WindowMain* SingletonManager::GetWindowMainSingleton()
{
    return mainBinaryWindowMain;
}

Time* SingletonManager::GetTimeSingleton()
{
    return mainBinaryTime;
}

void SingletonManager::SetInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM)
{
    SingletonManager::mainBinarySM = mainBinarySM;
}


void SingletonManager::InitFromMainBinary()
{
    SingletonManager::mainBinarySM = new SingletonManager();
}

SingletonManager* SingletonManager::GetInstance()
{
    return SingletonManager::mainBinarySM;
}

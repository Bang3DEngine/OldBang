#include "SingletonManager.h"

SingletonManager *SingletonManager::mainBinarySM = nullptr;


void SingletonManager::SetWindowMainSingleton(WindowMain *mainBinaryWindowMain)
{
    this->mainBinaryWindowMain = mainBinaryWindowMain;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryTime)
{
    this->mainBinaryTime = mainBinaryTime;
}

void SingletonManager::SetInputSingleton(Input *mainBinaryInput)
{
    this->mainBinaryInput = mainBinaryInput;
}





WindowMain* SingletonManager::GetWindowMainSingleton()
{
    return mainBinaryWindowMain;
}

Time* SingletonManager::GetTimeSingleton()
{
    return mainBinaryTime;
}

Input *SingletonManager::GetInputSingleton()
{
    return mainBinaryInput;
}

void SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM)
{
    SingletonManager::mainBinarySM = mainBinarySM;
}


void SingletonManager::InitSingletonMangerFromMainBinary()
{
    SingletonManager::mainBinarySM = new SingletonManager();
}

SingletonManager* SingletonManager::GetInstance()
{
    return SingletonManager::mainBinarySM;
}

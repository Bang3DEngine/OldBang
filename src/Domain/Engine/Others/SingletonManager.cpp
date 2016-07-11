#include "SingletonManager.h"

SingletonManager *SingletonManager::s_mainBinarySM = nullptr;


void SingletonManager::SetWindowMainSingleton(WindowMain *mainBinaryWindowMain)
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





WindowMain* SingletonManager::GetWindowMainSingleton()
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

void SingletonManager::SetSingletonManagerInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM)
{
    SingletonManager::s_mainBinarySM = mainBinarySM;
}


void SingletonManager::InitSingletonMangerFromMainBinary()
{
    SingletonManager::s_mainBinarySM = new SingletonManager();
}

SingletonManager* SingletonManager::GetInstance()
{
    return SingletonManager::s_mainBinarySM;
}

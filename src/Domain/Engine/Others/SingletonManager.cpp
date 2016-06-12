#include "SingletonManager.h"

SingletonManager *SingletonManager::mainBinarySM = nullptr;


void SingletonManager::SetWindowMainSingleton(WindowMain *mainBinaryWM)
{
    mainBinaryWindowMain = mainBinaryWM;
}

void SingletonManager::SetLoggerSingleton(Logger *mainBinaryL)
{
    mainBinaryLogger = mainBinaryL;
}

void SingletonManager::SetTimeSingleton(Time *mainBinaryT)
{
    mainBinaryTime = mainBinaryT;
}




WindowMain* SingletonManager::GetWindowMainSingleton()
{
    return mainBinaryWindowMain;
}

Logger* SingletonManager::GetLoggerSingleton()
{
    return mainBinaryLogger;
}

Time* SingletonManager::GetTimeSingleton()
{
    return mainBinaryTime;
}

SingletonManager* SingletonManager::
    SetInstanceFromBehaviourLibrary(SingletonManager *mainBinarySM)
{
    SingletonManager::mainBinarySM = mainBinarySM;
}


SingletonManager *SingletonManager::Init()
{
    SingletonManager::mainBinarySM = new SingletonManager();
}

SingletonManager* SingletonManager::GetInstance()
{
    return SingletonManager::mainBinarySM;
}

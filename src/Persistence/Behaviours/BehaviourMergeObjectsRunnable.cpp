#include "BehaviourMergeObjectsRunnable.h"

#include "Time.h"
#include "List.h"
#include "Debug.h"
#include "SystemUtils.h"
#include "Persistence.h"
#include "BehaviourManager.h"

BehaviourMergeObjectsRunnable::BehaviourMergeObjectsRunnable()
{

}

void BehaviourMergeObjectsRunnable::run()
{
    Merge();
}

void BehaviourMergeObjectsRunnable::Merge()
{
    Debug_Log("Merge();");

    List<String> behaviourObjects =
            BehaviourManager::GetBehavioursObjectsFilepathsList();
    String libOutputFilepath =
            Persistence::GetProjectLibsRootAbs() + "/Behaviours.so";
    libOutputFilepath += "." + std::to_string(Time::GetNow()) + ".1.1";

    typedef SystemUtils::CompilationFlags CLFlags;
    bool successCompiling; String output;
    SystemUtils::Compile(behaviourObjects, libOutputFilepath,
                         &successCompiling, &output,
                           CLFlags::AddEngineObjectFiles  |
                           CLFlags::AddProjectObjectFiles |
                           CLFlags::ProduceSharedLib);
    if (successCompiling)
    {
        Debug_Log("NotifySuccessMerging");
        emit NotifySuccessMerging(libOutputFilepath.ToQString(),
                                  output.ToQString());
    }
    else
    {
        Debug_Log("NotifyFailedMerging");
        emit  NotifyFailedMerging(output.ToQString());
    }
}


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
        emit NotifySuccessMerging(libOutputFilepath.ToQString(),
                                  output.ToQString());
    }
    else { emit NotifyFailedMerging(output.ToQString()); }
}


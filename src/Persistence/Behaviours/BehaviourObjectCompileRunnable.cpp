#include "BehaviourObjectCompileRunnable.h"

#include "Debug.h"
#include "SystemUtils.h"
#include "BehaviourManager.h"

BehaviourObjectCompileRunnable::BehaviourObjectCompileRunnable(
        const String &behaviourFilepath)
{
    m_behaviourFilepath = behaviourFilepath;
}

void BehaviourObjectCompileRunnable::run()
{
    CompileBehaviourObject();
}

void BehaviourObjectCompileRunnable::CompileBehaviourObject()
{
    Debug_Log("CompileBehaviourObject " << m_behaviourFilepath);

    String behaviourName = Persistence::GetFileName(m_behaviourFilepath);
    String objFilepath =
            Persistence::GetProjectLibsRootAbs() + "/" + behaviourName + ".o";

    bool successCompiling; String output;
    List<String> sources = {m_behaviourFilepath};
    SystemUtils::Compile(sources, objFilepath, &successCompiling, &output);
    if (successCompiling)
    {
        Debug_Log("NotifySuccessCompiling");
        emit NotifySuccessCompiling(m_behaviourFilepath.ToQString(),
                                    output.ToQString());
    }
    else
    {
        Debug_Log("NotifyFailedCompiling");
        emit NotifyFailedCompiling(m_behaviourFilepath.ToQString(),
                                   output.ToQString());
    }
}

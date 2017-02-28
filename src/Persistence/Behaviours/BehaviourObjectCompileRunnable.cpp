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
    String behaviourName = Persistence::GetFileName(m_behaviourFilepath);
    String objFilepath =
            Persistence::GetProjectLibsRootAbs() + "/" + behaviourName + ".o";

    bool successCompiling = false; String output = "";
    List<String> sources = {m_behaviourFilepath};
    SystemUtils::Compile(sources, objFilepath, &successCompiling, &output);
    if (successCompiling)
    {
        emit NotifySuccessCompiling(m_behaviourFilepath.ToQString(),
                                    output.ToQString());
    }
    else
    {
        emit NotifyFailedCompiling(m_behaviourFilepath.ToQString(),
                                   output.ToQString());
    }
}

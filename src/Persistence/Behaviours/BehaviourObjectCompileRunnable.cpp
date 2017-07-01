#include "Bang/BehaviourObjectCompileRunnable.h"

#include "Bang/Debug.h"
#include "Bang/BangCompiler.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BehaviourManager.h"

BehaviourObjectCompileRunnable::BehaviourObjectCompileRunnable(
        const Path &behaviourFilepath, bool forGame)
{
    m_forGame = forGame;
    m_behaviourPath = behaviourFilepath;
}

void BehaviourObjectCompileRunnable::run()
{
    CompileBehaviourObject();
}

void BehaviourObjectCompileRunnable::CompileBehaviourObject()
{
    String behaviourName = m_behaviourPath.GetName();
    Path objOutFilepath = Path(BehaviourManager::GetCurrentLibsDir())
                               .Append(behaviourName).AppendExtension("o");
    File::Remove(objOutFilepath);

    Path headerPath = m_behaviourPath.GetDirectory().Append(
                                Path(m_behaviourPath.GetName())
                                .AppendExtension("h") );
    BangPreprocessor::Preprocess(headerPath);

    Compiler::Result compileResult;
    compileResult = BangCompiler::CompileBehaviourObject(m_behaviourPath,
                                                         objOutFilepath,
                                                         m_forGame);
    if (compileResult.success)
    {
        emit NotifySuccessCompiling(m_behaviourPath.GetAbsolute().ToQString(),
                                    m_forGame,
                                    compileResult.output.ToQString());
    }
    else
    {
        emit NotifyFailedCompiling(m_behaviourPath.GetAbsolute().ToQString(),
                                   compileResult.output.ToQString());
    }
}

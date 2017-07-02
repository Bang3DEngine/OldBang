#include "Bang/BehaviourObjectCompileRunnable.h"

#include "Bang/Debug.h"
#include "Bang/BangCompiler.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BehaviourManager.h"

BehaviourObjectCompileRunnable::BehaviourObjectCompileRunnable(
        const Path &behaviourFilepath,
        const Path &outputObjectDir,
        bool forGame)
{
    m_forGame = forGame;
    m_outputObjectDir = outputObjectDir;
    m_behaviourPath = behaviourFilepath;
}

void BehaviourObjectCompileRunnable::run()
{
    String behaviourName = m_behaviourPath.GetName();
    Path objOutFilepath = m_outputObjectDir.Append(behaviourName)
                                           .AppendExtension("o");
    File::Remove(objOutFilepath);

    Path headerPath = m_behaviourPath.ChangeExtension("h");
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

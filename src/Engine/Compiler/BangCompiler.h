#ifndef BANGCOMPILER_H
#define BANGCOMPILER_H

#include <QThreadPool>

#include "Bang/BinType.h"
#include "Bang/Compiler.h"

class BangCompiler
{
public:
    struct Job : public Compiler::Job
    {
        bool forGame;
        BinType binaryType;
    };

    struct Result : public Compiler::Result
    {
        bool forGame;

        Result() {}
        explicit Result(const Compiler::Result &result)
        {
            compileJob = result.compileJob;
            output     = result.output;
            success    = result.success;
        }
    };


    static BangCompiler::Result MergeBehaviourLibraries(
                                            const List<Path> &objectsPaths,
                                            const Path &outputLibPath,
                                            bool forGame,
                                            BinType binaryType);
    static BangCompiler::Result CompileBehaviourObject(
                                                const Path &behaviourPath,
                                                const Path &outputObjectPath,
                                                bool forGame,
                                                BinType binaryType);

    static BangCompiler::Result Compile(const BangCompiler::Job &job);

    static bool MergeBehaviourLibrariesAsync(const List<Path> &objectsPaths,
                                             const Path &outputLibDir,
                                             bool forGame,
                                             BinType binaryType,
                                             QObject *resultListener,
                                             QSlot slot);
    static bool CompileBehaviourObjectAsync(const Path &behaviourPath,
                                            const Path &outputObjectDir,
                                            bool forGame,
                                            BinType binaryType,
                                            QObject *resultListener,
                                            QSlot slot);

    static bool QMakeMake(const Path &proFilepath,
                          const List<String> &qmakeArgs);


    static void Init();

private:
    static BangCompiler::Job BuildCompileBehaviourObjectJob(
                                               const Path &behaviourPath,
                                               const Path &outputObjectDir,
                                               bool forGame,
                                               BinType binaryType);
    static BangCompiler::Job BuildMergeBehaviourLibrariesJob(
                                                const List<Path> &objectsPaths,
                                                const Path &outputLibDir,
                                                bool forGame,
                                                BinType binaryType);
    static BangCompiler::Job BuildCommonJob(bool forGame,
                                            BinType binaryType);

    static QThreadPool* GetThreadPool();

    BangCompiler();
};

Q_DECLARE_METATYPE(BangCompiler::Result)

#endif // BANGCOMPILER_H

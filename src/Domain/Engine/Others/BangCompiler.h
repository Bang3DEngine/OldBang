#ifndef BANGCOMPILER_H
#define BANGCOMPILER_H

#include "Bang/Compiler.h"

class BangCompiler
{
public:
    static Compiler::Result MergeBehaviourLibraries(
                                            const List<Path> &objectsPaths,
                                            const Path &outputLibPath,
                                            bool forGame);
    static Compiler::Result CompileBehaviourObject(const Path &behaviourPath,
                                                   const Path &outputObjectPath,
                                                   bool forGame);

private:
    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetAllEngineSubDirs();

    static List<Path> GetQtIncludeDirs();
    static List<Path> GetQtLibrariesDirs();

    static Compiler::Job BuildCommonJob(bool forGame);

    BangCompiler();
};

#endif // BANGCOMPILER_H

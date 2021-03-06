#ifndef COMPILER_H
#define COMPILER_H

#include "Bang/List.h"
#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

class Compiler
{
public:
    enum OutputType
    {
        Object,
        SharedLib,
        Executable
    };

    struct Job
    {
        List<Path> inputFiles; // cpp or objects
        List<Path> includePaths;
        List<Path> libDirs;
        List<String> libraries;
        OutputType outputMode;
        List<String> flags;
        Path outputFile;
    };

    struct Result
    {
        Job compileJob;
        bool success;
        String output;
    };

    static Result Compile(const Job &compileJob);

private:
    Compiler();
};

NAMESPACE_BANG_END

#endif // COMPILER_H

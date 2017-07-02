#ifndef COMPILER_H
#define COMPILER_H

#include "Bang/List.h"
#include "Bang/Path.h"

class Compiler
{
public:
    enum OutputMode
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
        OutputMode outputMode;
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

#endif // COMPILER_H

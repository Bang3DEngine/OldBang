#include "Bang/Compiler.h"

#include "Bang/Debug.h"
#include "Bang/SystemUtils.h"
#include "Bang/BangPreprocessor.h"

USING_NAMESPACE_BANG

Compiler::Result Compiler::Compile(const Compiler::Job &job)
{
    List<String> args;

    // Output mode
    if      (job.outputMode == OutputType::Object)     { args.PushBack("-c"); }
    else if (job.outputMode == OutputType::SharedLib)  { args.PushBack("-shared"); }
    else if (job.outputMode == OutputType::Executable) { args.PushBack("-c"); }

    // Flags
    args.PushBack(job.flags);

    // Input Files
    args.PushBack(job.inputFiles.To<List,String>());

    // Include paths
    List<String> incPaths = job.includePaths.To<List,String>();
    for (String &incPath : incPaths) { incPath.Prepend("-I"); }
    args.PushBack(incPaths);

    // Library directories
    List<String> libDirs = job.libDirs.To<List,String>();
    for (String &libDir : libDirs) { libDir.Prepend("-L"); }
    args.PushBack(libDirs);

    // Libraries
    List<String> libs = job.libraries.To<List,String>();
    for (String &lib : libs) { lib.Prepend("-l"); }
    args.PushBack(libs);

    // Output file
    args.PushBack( List<String>({"-o", job.outputFile.ToString()}) );


    Result result;
    result.compileJob = job;
    SystemUtils::System("/usr/bin/g++",
                        args,
                        &result.output,
                        &result.success);

    return result;
}

Compiler::Compiler()
{
}

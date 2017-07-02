#include "Bang/Compiler.h"

#include "Bang/SystemUtils.h"

Compiler::Result Compiler::Compile(const Compiler::Job &job)
{
    List<String> args;

    // Output mode
    if      (job.outputMode == OutputMode::Object)     { args.Add("-c"); }
    else if (job.outputMode == OutputMode::SharedLib)  { args.Add("-shared"); }
    else if (job.outputMode == OutputMode::Executable) { args.Add("-c"); }

    // Flags
    args.Add(job.flags);

    // Input Files
    args.Add(job.inputFiles.To<List,String>());

    // Include paths
    List<String> incPaths = job.includePaths.To<List,String>();
    for (String &incPath : incPaths) { incPath.Prepend("-I"); }
    args.Add(incPaths);

    // Library directories
    List<String> libDirs = job.libDirs.To<List,String>();
    for (String &libDir : libDirs) { libDir.Prepend("-L"); }
    args.Add(libDirs);

    // Libraries
    List<String> libs = job.libraries.To<List,String>();
    for (String &lib : libs) { lib.Prepend("-l"); }
    args.Add(libs);

    // Output file
    args.Add( List<String>({"-o", job.outputFile.ToString()}) );

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

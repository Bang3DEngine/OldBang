#include "BangCompiler.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/SystemUtils.h"
#include "Bang/BehaviourManager.h"


List<Path> BangCompiler::GetAllProjectSubDirs()
{
    List<Path> subdirs = Paths::Project().GetSubDirectories(true);
    subdirs.PushFront(Paths::Project());
    return subdirs;
}

List<Path> BangCompiler::GetAllEngineSubDirs()
{
    List<Path> subdirs = Paths::Engine().GetSubDirectories(true);
    subdirs.PushFront(Paths::Engine());
    return subdirs;
}

List<Path> BangCompiler::GetQtIncludeDirs()
{
    bool ok = false;
    String qtDir = "";
    SystemUtils::System("qmake", {"-query", "QT_INSTALL_HEADERS"}, &qtDir, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt include directories to compile.");
        return {};
    }
    qtDir = qtDir.Replace("\n", "");

    List<Path> incs = Path(qtDir).GetSubDirectories(true);
    for (auto it = incs.Begin(); it != incs.End();)
    {
        const Path& libPath = *it;
        const String& incDir = libPath.GetAbsolute();
        if (!incDir.Contains("qt", false) || !libPath.IsDir())
        {
            incs.Remove(it++);
        }
        else { ++it; }
    }
    incs.PushFront( Path(qtDir) );
    return incs;
}

List<Path> BangCompiler::GetQtLibrariesDirs()
{
    bool ok = false;
    String qtDir = "";
    SystemUtils::System("qmake", {"-query", "QT_INSTALL_LIBS"}, &qtDir, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt library directories to compile.");
        return {};
    }
    qtDir = qtDir.Replace("\n", "");

    List<Path> libs = Path(qtDir).GetSubDirectories(true);
    for (auto it = libs.Begin(); it != libs.End();)
    {
        const Path& libDirPath = *it;
        const String& libDir = libDirPath.GetAbsolute();
        if ( !libDir.Contains("qt", false) || !libDirPath.IsDir() )
        {
            it = libs.Remove(it++);
        }
        else { ++it; }
    }
    libs.PushFront( Path(qtDir) );
    return libs;
}

Compiler::Job BangCompiler::BuildCommonJob(bool forGame)
{
    Compiler::Job job;


    job.libDirs.Add(forGame ? Paths::GameExecutableLibrariesDir() :
                              Paths::EngineLibrariesDir());
    job.libDirs.Add( BangCompiler::GetQtLibrariesDirs() );
    job.libraries.Add( List<String>({"GLEW", "GL", "pthread",
                                     "Bang", "BangDataStructures"}) );

    job.includePaths.Add( BangCompiler::GetAllEngineSubDirs() );
    job.includePaths.Add( BangCompiler::GetAllProjectSubDirs() );
    job.includePaths.Add( BangCompiler::GetQtIncludeDirs() );

    job.flags =  {"-O0", "-g",
                  "-Wl,-O0,--export-dynamic",
                  "-fPIC", "--std=c++11"};

    if (!forGame) { job.flags.Add("-DBANG_EDITOR"); }

    return job;
}

Compiler::Result BangCompiler::MergeBehaviourLibraries(
                                            const List<Path> &objectsPaths,
                                            const Path &outputLibPath,
                                            bool forGame)
{
    Compiler::Job job = BuildCommonJob(forGame);
    job.outputMode = Compiler::OutputMode::SharedLib;
    job.inputFiles.Add(objectsPaths);
    job.outputFile = outputLibPath;

    Path libsDir = BehaviourManager::GetCurrentLibsDir();
    File::CreateDirectory(libsDir);

    return Compiler::Compile(job);
}

Compiler::Result BangCompiler::CompileBehaviourObject(
                                            const Path &behaviourPath,
                                            const Path &outputObjectPath,
                                            bool forGame)
{
    Compiler::Job job = BuildCommonJob(forGame);
    job.outputMode = Compiler::OutputMode::Object;
    job.inputFiles.Add(behaviourPath);
    job.outputFile = outputObjectPath;

    return Compiler::Compile(job);
}

BangCompiler::BangCompiler()
{

}

#include "Bang/BangCompiler.h"

#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/SystemUtils.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BangCompilerAsyncJob.h"

BangCompiler::Job BangCompiler::BuildCompileBehaviourObjectJob(
                                            const Path &behaviourPath,
                                            const Path &outputObjectDir,
                                            bool forGame,
                                            BinType binaryType)
{
    Path outputObjectPath = outputObjectDir.Append(behaviourPath.GetName())
                                           .AppendExtension("o");
    File::Remove(outputObjectPath);
    File::CreateDirectory(outputObjectDir);

    BangCompiler::Job job = BuildCommonJob(forGame, binaryType);
    job.outputMode = Compiler::OutputMode::Object;
    job.includePaths.Add( Paths::GetAllEngineSubDirs() );
    job.includePaths.Add( Paths::GetAllProjectSubDirs() );
    job.includePaths.Add( Paths::GetQtIncludeDirs() );
    job.inputFiles.Add(behaviourPath);
    job.outputFile = outputObjectPath;
    return job;
}

BangCompiler::Job BangCompiler::BuildMergeBehaviourLibrariesJob(
                                            const List<Path> &objectsPaths,
                                            const Path &outputLibDir,
                                            bool forGame,
                                            BinType binaryType)
{
    // Remove merged library files
    List<Path> libFilepaths = outputLibDir.GetFiles(true, {"*.so.*"});
    for (const Path &libFilepath : libFilepaths) { File::Remove(libFilepath); }

    Path libOutputFilepath = outputLibDir.Append("Behaviours")
                                         .AppendExtension("so");
    String version = "";
    if (!forGame) { version = String::ToString( Time::GetNow() ); }
    else
    {
        Path gameProjectDir = libOutputFilepath.GetDirectory().GetDirectory();
        Path gameProjectFilepath = gameProjectDir.Append("Game")
                                                 .AppendExtension("bproject");

        Project gameProject;
        gameProject.ReadFromFile(gameProjectFilepath);
        version = gameProject.GetProjectRandomId();
    }
    libOutputFilepath = libOutputFilepath.AppendExtension(version + ".1.1");

    File::CreateDirectory(outputLibDir);

    BangCompiler::Job job = BuildCommonJob(forGame, binaryType);
    job.outputMode = Compiler::OutputMode::SharedLib;
    job.inputFiles.Add(objectsPaths);
    job.outputFile = libOutputFilepath;

    return job;
}

BangCompiler::Job BangCompiler::BuildCommonJob(bool forGame,
                                               BinType binaryType)
{
    BangCompiler::Job job;

    job.libDirs.Add(forGame ? Paths::GameExecutableLibrariesDir(binaryType) :
                              Paths::EngineLibrariesDir(binaryType));
    job.libraries.Add( List<String>({"GLEW", "GL", "pthread",
                                     "BangDataStructures",
                                     "BangGraphics",
                                     "BangEngine",
                                     "BangDataStructures",
                                     "BangGraphics",
                                     "BangEngine"}) );

    job.flags =  {"-O0", "-g",
                  "-Wl,-O0,--export-dynamic",
                  "-fPIC", "--std=c++11"};

    job.forGame = forGame;

    return job;
}

QThreadPool* BangCompiler::GetThreadPool()
{
    static bool init = false;
    static QThreadPool s_threadPool;
    if (!init)
    {
        s_threadPool.setMaxThreadCount(1);
        init = true;
    }
    return &s_threadPool;
}

BangCompiler::Result BangCompiler::MergeBehaviourLibraries(
                                            const List<Path> &objectsPaths,
                                            const Path &outputLibPath,
                                            bool forGame,
                                            BinType binaryType)
{
    BangCompiler::Job job = BuildMergeBehaviourLibrariesJob(objectsPaths,
                                                            outputLibPath,
                                                            forGame,
                                                            binaryType);
    return BangCompiler::Compile(job);
}

BangCompiler::Result BangCompiler::CompileBehaviourObject(
                                            const Path &behaviourPath,
                                            const Path &outputObjectPath,
                                            bool forGame,
                                            BinType binaryType)
{
    BangCompiler::Job job = BuildCompileBehaviourObjectJob({behaviourPath},
                                                           outputObjectPath,
                                                           forGame,
                                                           binaryType);
    return BangCompiler::Compile(job);
}

BangCompiler::Result BangCompiler::Compile(const BangCompiler::Job &job)
{
    for (const Path &srcPath : job.inputFiles)
    {
        if (srcPath.HasExtension("cpp") || srcPath.HasExtension("h"))
        {
            Path headerPath = srcPath.ChangeExtension("h");
            BangPreprocessor::Preprocess(headerPath);
        }
    }

    Compiler::Result baseResult = Compiler::Compile(job);
    BangCompiler::Result result(baseResult);
    result.forGame = job.forGame;
    return result;
}

bool BangCompiler::MergeBehaviourLibrariesAsync(const List<Path> &objectsPaths,
                                                const Path &outputLibDir,
                                                bool forGame,
                                                BinType binaryType,
                                                QObject *resultListener,
                                                QSlot slot)
{
    BangCompiler::Job job = BuildMergeBehaviourLibrariesJob(objectsPaths,
                                                            outputLibDir,
                                                            forGame,
                                                            binaryType);
    BangCompilerAsyncJob *asyncJob = new BangCompilerAsyncJob(job,
                                                              resultListener,
                                                              slot);
    bool started = BangCompiler::GetThreadPool()->tryStart(asyncJob);
    if (!started) { delete asyncJob; }
    return started;
}

bool BangCompiler::CompileBehaviourObjectAsync(const Path &behaviourPath,
                                               const Path &outputObjectDir,
                                               bool forGame,
                                               BinType binaryType,
                                               QObject *resultListener,
                                               QSlot slot)
{
    BangCompiler::Job job = BuildCompileBehaviourObjectJob(behaviourPath,
                                                           outputObjectDir,
                                                           forGame,
                                                           binaryType);
    BangCompilerAsyncJob *asyncJob = new BangCompilerAsyncJob(job,
                                                              resultListener,
                                                              slot);
    bool started = BangCompiler::GetThreadPool()->tryStart(asyncJob);
    if (!started) { delete asyncJob; }
    return started;
}

bool BangCompiler::QMakeMake(const Path &proFilepath,
                             const List<String> &qmakeArgs)
{
    String cmd = "";
    cmd += "cd " + proFilepath.GetDirectory()  + " && ";
    cmd += "qmake " + proFilepath.GetNameExt() +
            " " + String::Join(qmakeArgs, " ") + " && ";
    cmd += "make";

    String output = "";
    bool success = false;
    SystemUtils::System(cmd, {}, &output, &success);

    if (!success) { Debug_Error(output); }
    else { Debug_Warn(output); }

    return success;
}

void BangCompiler::Init()
{
    qRegisterMetaType<BangCompiler::Result>("BangCompiler::Result");
}

BangCompiler::BangCompiler()
{
}

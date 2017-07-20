#include "Bang/BehaviourManager.h"

#include <QLibrary>

#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
#include "Bang/CodePreprocessor.h"

BehaviourManager::BehaviourManager()
{
}

Compiler::Result BehaviourManager::CompileBehaviourObject(
                                        const Path &behaviourFilepath,
                                        const Path &outputObjectFilepath,
                                        BinType binaryType)
{
    File::Remove(outputObjectFilepath);
    File::CreateDirectory(outputObjectFilepath.GetDirectory());

    Compiler::Job job = BehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::Object;
    job.includePaths.Add( Paths::GetAllEngineSubDirs() );
    job.includePaths.Add( Paths::GetAllProjectSubDirs() );
    job.includePaths.Add( Paths::GetQtIncludeDirs() );
    job.inputFiles.Add(behaviourFilepath);
    job.outputFile = outputObjectFilepath;

    return Compiler::Compile(job);
}

Compiler::Result BehaviourManager::MergeBehaviourObjects(
                                    const Path &outputLibFilepath,
                                    const List<Path> &behaviourObjectFilepaths,
                                    BinType binaryType)
{
    File::CreateDirectory(outputLibFilepath.GetDirectory());

    Compiler::Job job = BehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::SharedLib;
    job.inputFiles.Add(behaviourObjectFilepaths);
    job.outputFile = outputLibFilepath;;

    return Compiler::Compile(job);
}

QLibrary *BehaviourManager::GetBehavioursLibrary()
{
    return BehaviourManager::GetInstance()->m_behavioursLibrary;
}

void BehaviourManager::LoadBehavioursLibrary(const Path &behavioursLibrary)
{
    QLibrary *behLib = new QLibrary(behavioursLibrary.ToString().ToQString());
    behLib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);

    bool success = behLib->load();
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_behavioursLibrary = success ? behLib : nullptr;
    if (!success)
    {
        Debug_Error("There was an error when loading the library '" <<
                     behavioursLibrary << "': " << behLib->errorString());
    }
}

void BehaviourManager::RemoveOldBehaviourLibraries(const Path &librariesDir)
{
    // Remove old library filepaths
    List<Path> libFilepaths = librariesDir.FindFiles(true, {"*.so.*"});
    for (const Path &libFilepath : libFilepaths) { File::Remove(libFilepath); }
}

Compiler::Job BehaviourManager::CreateBaseJob(BinType binaryType)
{
    Compiler::Job job;
    job.libDirs.Add(Paths::EngineLibrariesDir(binaryType));
    job.libraries.Add( List<String>({"GLEW",
                                     "GL",
                                     "pthread",
                                     "BangDataStructures",
                                     "BangGraphics",
                                     "BangEngine",
                                     "BangDataStructures",
                                     "BangGraphics",
                                     "BangEngine"}) );

    job.flags =  {"-fPIC", "--std=c++11"};
    if (binaryType == BinType::Debug)
    {
        job.flags.Add( List<String>({"-O0", "-g", "-Wl,-O0,--export-dynamic"}) );
    }
    else
    {
        job.flags.Add( List<String>({"-O3", "-Wl,-O3,--export-dynamic"}) );
    }

    return job;
}

BehaviourManager *BehaviourManager::GetInstance()
{
    return Application::GetInstance()->GetBehaviourManager();
}

#include "Bang/BehaviourManager.h"


#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/Project.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"

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
    job.includePaths.PushBack( Paths::GetEngineIncludeDirs() );
    job.includePaths.PushBack( Paths::GetProjectIncludeDirs() );
    job.inputFiles.PushBack(behaviourFilepath);
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
    job.inputFiles.PushBack(behaviourObjectFilepaths);
    job.outputFile = outputLibFilepath;;

    return Compiler::Compile(job);
}

Library *BehaviourManager::GetBehavioursLibrary()
{
    return BehaviourManager::GetInstance()->m_behavioursLibrary;
}

void BehaviourManager::LoadBehavioursLibrary(const Path &behavioursLibrary)
{
    Library *behLib = new Library(behavioursLibrary);

    bool success = behLib->Load();
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_behavioursLibrary = success ? behLib : nullptr;
    if (!success)
    {
        Debug_Error("There was an error when loading the library '" <<
                     behavioursLibrary << "': " << behLib->GetErrorString());
    }
}

void BehaviourManager::RemoveOldBehaviourLibraries(const Path &librariesDir)
{
    // Remove old library filepaths
    List<Path> libFilepaths = librariesDir.FindFiles(Path::FindFlag::Recursive);
    for (const Path &libFilepath : libFilepaths)
    {
        if (libFilepath.GetAbsolute().Contains(".so."))
        {
            File::Remove(libFilepath);
        }
    }
}

Compiler::Job BehaviourManager::CreateBaseJob(BinType binaryType)
{
    Compiler::Job job;
    job.libDirs.PushBack(Paths::EngineLibrariesDir(binaryType));
    job.libraries.PushBack( List<String>({"Bang"}) );

    job.flags =  {"-fPIC", "--std=c++11"};
    if (binaryType == BinType::Debug)
    {
        job.flags.PushBack( List<String>({"-O0", "-g", "-Wl,-O0,--export-dynamic"}) );
    }
    else
    {
        job.flags.PushBack( List<String>({"-O3", "-Wl,-O3,--export-dynamic"}) );
    }

    return job;
}

BehaviourManager *BehaviourManager::GetInstance()
{
    return Application::GetInstance()->GetBehaviourManager();
}

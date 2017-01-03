#include "BehaviourManager.h"

#include <QLibrary>

#include "Debug.h"
#include "Debug.h"
#include "Persistence.h"
#include "Application.h"
#include "SystemUtils.h"
#include "BehaviourHolder.h"

#include "BehaviourManagerCompileThread.h"

BehaviourManager::BehaviourManager()
{
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

// Called by the BehaviourManagerCompileThread when has finished
void BehaviourManager::OnBehaviourFinishedCompiling(const String &behaviourPath,
                                                    const String &libraryFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);

    if (bm->m_behPathsBeingCompiled.find(absPath) !=
        bm->m_behPathsBeingCompiled.end())
    {
        bm->m_behPathsBeingCompiled.erase(bm->m_behPathsBeingCompiled.find(absPath));
    }

    // Open the library
    String hash = Persistence::GetHash(absPath);
    QLibrary *lib = new  QLibrary(libraryFilepath.ToCString());
    lib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
    if (lib->load())
    {
        bm->m_behaviourHash_To_library[hash] = lib;

        // Notify the BehaviourHolder
        List<BehaviourHolder*> behDemanders =
                bm->m_behHash_To_behHolderDemanders[hash];

        for (BehaviourHolder* bh : behDemanders)
        {
            bh->OnBehaviourLibraryAvailable(lib);
        }

        bm->m_behHash_To_behHolderDemanders.Remove(hash);

        // Remove the outdated libraries files
        RemoveOutdatedLibraryFiles(libraryFilepath);
    }
    else
    {
        Debug_Error("There was an error when loading the library '" <<
                    libraryFilepath << "': " << lib->errorString().toStdString());
    }
}

void BehaviourManager::RemoveOutdatedLibraryFiles(const String &mostRecentLibraryFilepath)
{
    String libraryNameAndExtension =
            Persistence::GetFileNameWithExtension(mostRecentLibraryFilepath);
    String libraryName = Persistence::GetFileName(mostRecentLibraryFilepath);

    List<String> libs = Persistence::GetFiles(Persistence::c_ProjectAssetsRootAbsolute,
                                              true, {"*.so.*"});
    for (auto it = libs.Begin(); it != libs.End(); ++it)
    {
        const String &libFilepath = *it;
        bool isOfTheSameBehaviour = (Persistence::GetFileName(libFilepath) == libraryName);
        bool isTheMostRecentOne = libFilepath.EndsWith(libraryNameAndExtension);
        if (isOfTheSameBehaviour && !isTheMostRecentOne)
        {
            Persistence::Remove(libFilepath); // Remove outdated library
        }
    }
}

bool BehaviourManager::IsCached(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm, "", return false);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behaviourHash_To_library.ContainsKey(hash);
}

QLibrary *BehaviourManager::GetCachedLibrary(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm, "", return nullptr);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behaviourHash_To_library[hash];
}

bool BehaviourManager::IsBeingCompiled(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm, "", return false);
    String absPath = Persistence::ToAbsolute(behaviourPath, false);

    return bm->m_behPathsBeingCompiled.find(absPath) !=
           bm->m_behPathsBeingCompiled.end();
}

void BehaviourManager::Load(BehaviourHolder *behaviourHolder,
                            const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    String absPath = Persistence::ToAbsolute(behaviourFilepath, false);
    String hash = Persistence::GetHash(absPath);

    if (BehaviourManager::IsCached(absPath))
    {
        // It's cached from a previous load...
        QLibrary *lib = BehaviourManager::GetCachedLibrary(absPath);
        behaviourHolder->OnBehaviourLibraryAvailable(lib);
    }
    else
    {
        // Add behaviour to the list of demanders
        if (!bm->m_behHash_To_behHolderDemanders.ContainsKey(hash))
        {   // Init list
            bm->m_behHash_To_behHolderDemanders[hash] = List<BehaviourHolder*>();
        }
        bm->m_behHash_To_behHolderDemanders[hash].PushBack(behaviourHolder);

        if (!BehaviourManager::IsBeingCompiled(absPath))
        {
            // Compile once
            bm->m_behPathsBeingCompiled.insert(absPath);

            // Have to compile and load it. First compile
            BehaviourManagerCompileThread *compileThread =
                    new BehaviourManagerCompileThread(absPath);
            compileThread->start();

            Debug_Status("Compiling script " <<
                         Persistence::GetFileName(behaviourFilepath) << "...", 5.0f);

            // And when the compileThread finishes, we will be notified,
            // load the library, and then notify the behaviourHolders waiting for it
        }
    }
}

void BehaviourManager::
    OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    // Erase the behaviourHolder from all the demand lists it is in
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    for (auto it = bm->m_behHash_To_behHolderDemanders.Begin();
         it != bm->m_behHash_To_behHolderDemanders.End(); ++it)
    {
        List<BehaviourHolder*> &bhList = it->second;
        bhList.Remove(behaviourHolder);
    }
}

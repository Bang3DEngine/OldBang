#include "BehaviourManager.h"

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
void BehaviourManager::OnBehaviourFinishedCompiling(const String &behaviourRelPath,
                                                    const String &libraryFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); NONULL(bm);

    const String &bfp = behaviourRelPath;

    if (bm->m_behPathsBeingCompiled.find(bfp) !=
        bm->m_behPathsBeingCompiled.end())
    {
        bm->m_behPathsBeingCompiled.erase(bm->m_behPathsBeingCompiled.find(bfp));
    }

    // Open the library
    QLibrary *lib = new  QLibrary(libraryFilepath.ToCString());
    lib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
    if (lib->load())
    {
        bm->m_behaviourPath_To_library[behaviourRelPath] = lib;

        // Notify the BehaviourHolder
        List<BehaviourHolder*> behDemanders =
                bm->m_behPath_To_behHolderDemanders[bfp];

        for (BehaviourHolder* bh : behDemanders)
        {
            bh->OnBehaviourLibraryAvailable(lib);
        }

        bm->m_behPath_To_behHolderDemanders.erase(
                    bm->m_behPath_To_behHolderDemanders.find(bfp));
    }
    else
    {
        Debug_Error("There was an error when loading the library '" <<
                    libraryFilepath << "'");
    }
}

bool BehaviourManager::IsCached(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (!bm) { return false; }

    String bfp = Persistence::ToRelative(behaviourPath);
    return bm->m_behaviourPath_To_library.find(bfp) !=
           bm->m_behaviourPath_To_library.end();
}

QLibrary *BehaviourManager::GetCachedLibrary(const String &behaviourRelPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (!bm) { return nullptr; }

    return bm->m_behaviourPath_To_library[behaviourRelPath];
}

bool BehaviourManager::IsBeingCompiled(const String &behaviourRelPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (!bm) { return false; }

    return bm->m_behPathsBeingCompiled.find(behaviourRelPath) !=
           bm->m_behPathsBeingCompiled.end();
}

void BehaviourManager::Load(BehaviourHolder *behaviourHolder,
                            const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); NONULL(bm);

    String bfp = Persistence::ToRelative(behaviourFilepath);
    if (BehaviourManager::IsCached(bfp))
    {
        // It's cached from a previous load...
        QLibrary *lib = BehaviourManager::GetCachedLibrary(bfp);
        behaviourHolder->OnBehaviourLibraryAvailable(lib);
    }
    else
    {
        // Add behaviour to the list of demanders
        if (bm->m_behPath_To_behHolderDemanders.find(bfp) ==
            bm->m_behPath_To_behHolderDemanders.end())
        {
            bm->m_behPath_To_behHolderDemanders[bfp] =
                    List<BehaviourHolder*>(); // Init list
        }
        bm->m_behPath_To_behHolderDemanders[bfp].push_back(behaviourHolder);

        if (!BehaviourManager::IsBeingCompiled(bfp))
        {
            // Compile once

            bm->m_behPathsBeingCompiled.insert(bfp);

            // Have to compile and load it
            // First compile
            BehaviourManagerCompileThread *compileThread =
                    new BehaviourManagerCompileThread(bfp);
            compileThread->start(); // This auto-deletes itself when finished

            // And when the compileThread finishes, we will be notified,
            // load the library, and then notify the behaviourHolders waiting for it
        }
    }
}

void BehaviourManager::
    OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    // Erase the behaviourHolder from all the demand lists it is in
    BehaviourManager *bm = BehaviourManager::GetInstance(); NONULL(bm);

    for (auto it = bm->m_behPath_To_behHolderDemanders.begin();
         it != bm->m_behPath_To_behHolderDemanders.end(); ++it)
    {
        List<BehaviourHolder*> &bhList = it->second;
        bhList.remove(behaviourHolder);
    }
}

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

    Debug_Log("Finished compiling " << behaviourRelPath);
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
        std::list<BehaviourHolder*> behDemanders =
                bm->m_behPath_To_behHolderDemanders[bfp];

        bm->m_behPath_To_behHolderDemanders.erase(
                    bm->m_behPath_To_behHolderDemanders.find(bfp));

        for (BehaviourHolder* bh : behDemanders)
        {
            bh->OnBehaviourLibraryAvailable(lib);
        }
        bm->m_behPath_To_behHolderDemanders[bfp].clear();
    }
    else
    {
        Debug_Error("There was an error when loading the library '" <<
                    libraryFilepath << "'");
    }
}

void BehaviourManager::Load(BehaviourHolder *behaviourHolder,
                            const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); NONULL(bm);

    Debug_Log("Loading " << behaviourFilepath);

    String bfp = Persistence::ToRelative(behaviourFilepath);
    if (bm->m_behPathsBeingCompiled.find(bfp) !=
        bm->m_behPathsBeingCompiled.end())
    {
        return; // The BehaviourManager will notify it when available
    }

    if (bm->m_behaviourPath_To_library.find(bfp) !=
        bm->m_behaviourPath_To_library.end())
    {
        // It's cached from a previous load...
        QLibrary *lib = bm->m_behaviourPath_To_library[bfp];
        behaviourHolder->OnBehaviourLibraryAvailable(lib);
    }
    else
    {
        // Have to compile and load it
        // First compile
        BehaviourManagerCompileThread *compileThread =
                new BehaviourManagerCompileThread(bfp);
        compileThread->start(); // This auto-deletes itself when finished

        if (bm->m_behPath_To_behHolderDemanders.find(bfp) ==
            bm->m_behPath_To_behHolderDemanders.end())
        {
            bm->m_behPath_To_behHolderDemanders[bfp] =
                    std::list<BehaviourHolder*>();
        }
        bm->m_behPath_To_behHolderDemanders[bfp].push_back(behaviourHolder);

        // And when the compileThread finishes, we will be notified,
        // load it, and then notify the behaviourHolder
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
        std::list<BehaviourHolder*> &bhList = it->second;
        bhList.remove(behaviourHolder);
    }
}

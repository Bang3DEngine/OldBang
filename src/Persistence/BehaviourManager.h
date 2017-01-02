#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <set>

#include "Map.h"
#include "List.h"
#include "String.h"
#include "BehaviourRefresherTimer.h"

class QLibrary;
class BehaviourHolder;
class BehaviourManagerCompileThread;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the BehaviourHolder callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager
{

public:
    static bool IsBeingCompiled(const String &behaviourPath);

    // This must be called when you want to retrieve the QLibrary from a
    // behaviour source filepath
    static void Load(BehaviourHolder *behaviourHolder,
                     const String &behaviourFilepath);
    static void OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder);

    static BehaviourManager* GetInstance();
    static bool IsCached(const String &behaviourPath);

private:

    BehaviourRefresherTimer m_behaviourRefresherTimer;

    /**
     * @brief The cache of libraries. For the hash of the behaviour script
       *.cpp, contains its loaded library (if cached).
     */
    Map<String, QLibrary*> m_behaviourHash_To_library;

    /**
     * @brief For wololo.cpp, contains all the demanders of the
     * library for wololo.cpp. They will be notified when we
    Maphe library
     */
    Map<String, List<BehaviourHolder*> > m_behHash_To_behHolderDemanders;

    /**
     * @brief Set of behaviours abs paths that are being compiled right now.
     * This is useful in the case when a behaviour is demanded while it's
     * being compiled.
     */
    std::set<String> m_behPathsBeingCompiled;

    BehaviourManager();


    // Called by the BehaviourManagerCompileThread when has finished
    static void OnBehaviourFinishedCompiling(const String &behaviourPath,
                                             const String &soFilepath);

    static void RemoveOutdatedLibraryFiles(
            const String &mostRecentLibraryFilepath);
    static QLibrary* GetCachedLibrary(const String &behaviourPath);

    friend class Application;
    friend class BehaviourManagerCompileThread;
};

#endif // BEHAVIOURMANAGER_H

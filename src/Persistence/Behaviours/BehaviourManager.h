#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <set>
#include <QTimer>
#include <QMutex>
#include <QThreadPool>
#include <QMutexLocker>

#include "Map.h"
#include "List.h"
#include "String.h"
#include "BehaviourRefresherTimer.h"

class QLibrary;
class BehaviourHolder;
class BehaviourCompileRunnable;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the BehaviourHolder callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager : public QObject
{
    Q_OBJECT

public:

    // Refreshes all the behaviour holders without using async
    // Useful before playing for example, to ensure all the behaviours
    // are loaded

    static bool IsBeingCompiled(const String &hash);

    // This must be called when you want to retrieve the QLibrary from a
    // behaviour source filepath
    static void Load(BehaviourHolder *behaviourHolder,
                     const String &behaviourFilepath);

    static void OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder);

    static void RefreshAllBehaviours();

    static BehaviourManager* GetInstance();
    static bool IsCached(const String &hash);

    static bool AllBehaviourHoldersUpdated(float *percentOfBehavioursUpdated = nullptr);
    static bool SomeBehaviourWithError();

    static String GetHash(const String &sourceFilepath);

    // Called by the BehaviourManagerCompileThread when has finished
    void OnBehaviourFinishedCompiling(const String &behaviourPath,
                                      const String &soFilepath);

    // Called by the BehaviourManagerCompileThread when has failed
    void OnBehaviourFailedCompiling(const String &behaviourPath);

private:
    QMutex m_mutex;
    QThreadPool m_threadPool;

    BehaviourRefresherTimer m_behaviourRefresherTimer;

    /**
     * @brief The cache of libraries. For the hash of the behaviour script
       *.cpp, contains its loaded library (if cached).
     */
    Map<String, QLibrary*> m_behHash_To_lib;

    /**
     * @brief For wololo.cpp, contains all the demanders of the
     * library for wololo.cpp. They will be notified when we
    Maphe library
     */
    Map<String, List<BehaviourHolder*> > m_behHash_To_demandersList;

    /**
     * @brief Holds the compiled behaviour hashes that have failed to compile.
     */
    std::set<String> m_failedBehHashes;

    std::set<String> m_behHashesBeingCompiled;

    BehaviourManager();

    static QLibrary* LoadLibraryFromFilepath(const String &libFilepath);
    static void RemoveOutdatedLibraryFiles(
            const String &mostRecentLibraryFilepath);
    static QLibrary* GetCachedLibrary(const String &hash);

    friend class Application;
    friend class BehaviourCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

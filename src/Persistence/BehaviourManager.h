#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <map>
#include <set>
#include <QMutex>
#include <QLibrary>
#include <functional>

#include "Bang.h"
#include "String.h"

class BehaviourHolder;
class BehaviourManagerCompileThread;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the BehaviourHolder callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager
{
friend class Application;
friend class BehaviourManagerCompileThread;

private:

    /**
     * @brief The cache of libraries. For the source filepath
     * wololo.cpp, contains its loaded library (if cached).
     */
    std::map<String, QLibrary*> m_behaviourPath_To_library;

    /**
     * @brief For wololo.cpp, contains all the demanders of the
     * library for wololo.cpp. They will be notified when we
     * get the library
     */
    std::map<String, std::list<BehaviourHolder*> >
        m_behPath_To_behHolderDemanders;

    /**
     * @brief Set of behaviours paths that are being compiled right now.
     * This is useful in the case when a behaviour is demanded while it's
     * being compiled.
     */
    std::set<String> m_behPathsBeingCompiled;

    BehaviourManager();

    static BehaviourManager* GetInstance();

    // Called by the BehaviourManagerCompileThread when has finished
    static void OnBehaviourFinishedCompiling(const String &behaviourRelPath,
                                             const String &soFilepath);

    static bool IsCached(const String &behaviourRelPath);
    static QLibrary* GetCachedLibrary(const String &behaviourRelPath);

public:
    static bool IsBeingCompiled(const String &behaviourPath);

    // This must be called when you want to retrieve the QLibrary from a
    // behaviour source filepath
    static void Load(BehaviourHolder *behaviourHolder,
                     const String &behaviourFilepath);
    static void OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder);

};

#endif // BEHAVIOURMANAGER_H

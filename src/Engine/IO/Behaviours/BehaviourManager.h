#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <set>
#include <QTimer>
#include <QMutex>
#include <QLibrary>
#include <QMutexLocker>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/BangCompiler.h"
#include "Bang/BehaviourManagerStatus.h"

class QLibrary;
class Behaviour;
class BehaviourObjectCompileRunnable;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the Behaviour callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager : public QObject
{
    Q_OBJECT

public:
    static BehaviourManager* GetInstance();

    static QLibrary* GetBehavioursMergedLibrary();

    static bool PrepareBehavioursLibrary(bool forGame,
                                         const Path& outputLibDir,
                                         bool *stopFlag = nullptr);

    static const BehaviourManagerStatus& GetStatus();

signals:
    void NotifyPrepareBehavioursLibraryProgressed(int newProgress);


public slots: // Behaviour Objects signals and slots
    void OnBehaviourObjectCompilationFinished(const BangCompiler::Result &result);
    void OnMergingLibraryFinished(const BangCompiler::Result &result);

private:
    Path m_currentLibsDir;
    BehaviourManagerStatus m_status;

    bool m_mergingForGame = false;
    QLibrary *m_behavioursLibrary = nullptr;

    BehaviourManager();

    static bool StartMergingBehavioursObjects(bool forGame,
                                              const Path& outputLibDir);
    static void StartCompilingAllBehaviourObjects(bool forGame,
                                                  const Path& objectsDir);
    static void StartCompilingBehaviourObject(const Path &behaviourFilepath,
                                              const Path& objectsDir,
                                              bool forGame);

    friend class Behaviour;
    friend class Application;
    friend class BehaviourRefresherTimer;
    friend class BehaviourObjectCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

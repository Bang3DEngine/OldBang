#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <set>
#include <QTimer>
#include <QMutex>
#include <QLibrary>
#include <QThreadPool>
#include <QMutexLocker>

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Application.h"
#include "Bang/BehaviourManagerStatus.h"
#include "Bang/BehaviourRefresherTimer.h"

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
    enum MergingState { Idle, Merging, Success, Failed };

    static BehaviourManager* GetInstance();

    static QLibrary* GetBehavioursMergedLibrary();
    static List<Path> GetBehavioursSourcesFilepathsList();
    static List<Path> GetBehavioursObjectsFilepathsList();

    static bool PrepareBehavioursLibrary(bool forGame = false,
                                         bool *stopFlag = nullptr);

    static void SetCurrentLibsDir(const Path &currentLibsDir);
    static const Path &GetCurrentLibsDir();
    static MergingState GetMergeState();
    static const BehaviourManagerStatus& GetStatus();

signals:
    void NotifyPrepareBehavioursLibraryProgressed(int newProgress);

// Behaviour Objects signals and slots
public slots:
    void OnBehaviourObjectCompiled(const QString &behaviourFilepath,
                                   bool mergingForGame,
                                   const QString &warnMessage);
    void OnBehaviourObjectCompilationFailed(const QString &behaviourFilepath,
                                            const QString &errorMessage);

// Merging slots
private slots:
    void OnMergedLibraryCompiled(QString libFilepath,
                                 bool loadLibrary,
                                 QString warnMessage);
    void OnMergedLibraryCompilationFailed(QString errorMessage);

private:
    Path m_currentLibsDir;
    MergingState m_state = MergingState::Idle;
    BehaviourManagerStatus m_status;

    QThreadPool m_threadPool;
    BehaviourRefresherTimer m_behaviourRefresherTimer;

    bool m_mergingForGame = false;
    QLibrary *m_behavioursLibrary = nullptr;

    BehaviourManager();

    static void RemoveMergedLibraryFiles();

    static bool StartMergingBehavioursObjects(bool forGame = false);
    static void StartCompilingAllBehaviourObjects(bool forGame = false);
    static void StartCompilingBehaviourObject(const Path &behaviourFilepath,
                                              bool forGame = false);

    friend class Application;
    friend class Behaviour;
    friend class BehaviourRefresherTimer;
    friend class BehaviourObjectCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

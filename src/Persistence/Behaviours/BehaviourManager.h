#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <set>
#include <QTimer>
#include <QMutex>
#include <QLibrary>
#include <QThreadPool>
#include <QMutexLocker>

#include "Map.h"
#include "List.h"
#include "String.h"
#include "BehaviourManagerStatus.h"
#include "BehaviourRefresherTimer.h"

class QLibrary;
class BehaviourHolder;
class BehaviourObjectCompileRunnable;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the BehaviourHolder callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager : public QObject
{
    Q_OBJECT

public:
    enum MergingState { Idle, Merging, Success, Failed };

    static BehaviourManager* GetInstance();

    static QLibrary* GetBehavioursMergedLibrary();
    static List<String> GetBehavioursSourcesFilepathsList();
    static List<String> GetBehavioursObjectsFilepathsList();

    static bool PrepareBehavioursLibrary(bool *stopFlag = nullptr);

    static MergingState GetMergeState();
    static const BehaviourManagerStatus& GetStatus();

signals:
    void NotifyPrepareBehavioursLibraryProgressed(int newProgress);

// Behaviour Objects signals and slots
public slots:
    void OnBehaviourObjectCompiled(const QString &behaviourFilepath,
                                   const QString &warnMessage);
    void OnBehaviourObjectCompilationFailed(const QString &behaviourFilepath,
                                            const QString &errorMessage);

// Merging slots
private slots:
    void OnMergedLibraryCompiled(QString libFilepath, QString warnMessage);
    void OnMergedLibraryCompilationFailed(QString errorMessage);

private:
    MergingState m_state = MergingState::Idle;
    BehaviourManagerStatus m_status;

    QThreadPool m_threadPool;
    BehaviourRefresherTimer m_behaviourRefresherTimer;

    QLibrary *m_behavioursLibrary = nullptr;

    BehaviourManager();

    static void RemoveMergedLibraryFiles();

    static bool StartMergingBehavioursObjects();
    static void StartCompilingAllBehaviourObjects();
    static void StartCompilingBehaviourObject(const String &behaviourFilepath);

    friend class Application;
    friend class BehaviourHolder;
    friend class BehaviourRefresherTimer;
    friend class BehaviourObjectCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

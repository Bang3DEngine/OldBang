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
#include "BehaviourRefresherTimer.h"

class QLibrary;
class BehaviourHolder;
class BehavioursLibCompileRunnable;

/**
 * @brief Manages the compiling and loading of the Behaviour's QLibraries.
 * The compiling is asynchronous, so we must use the BehaviourHolder callback
 * to notify its QLibrary has been created.
 */
class BehaviourManager : public QObject
{
    Q_OBJECT

public:
    enum State
    {
        Idle,
        Compiling,
        Success,
        Failed
    };

    static BehaviourManager* GetInstance();

    /**
     * This must be called when you want to retrieve the QLibrary from a
     * behaviour source filepath.
    **/
    static void Load(const String &behaviourPath,
                     BehaviourHolder *behaviourHolder = nullptr);

    static QLibrary* GetBehavioursLibrary();
    static void RefreshBehavioursLibrary();

    static State GetState();

public slots:
    void OnBehavioursLibraryCompiled(const String &libFilepath,
                                     const String &warnMessage);
    void OnBehavioursLibraryCompilationFailed(const String &errorMessage);

private slots:
    void CompileBehavioursLibrary();

private:
    State m_state = State::Idle;
    QThread m_compileThread;
    //BehaviourRefresherTimer m_behaviourRefresherTimer;

    Map<String, String> m_behaviourPathToHash;
    QLibrary *m_behavioursLibrary = nullptr;

    BehaviourManager();

    static bool SomeBehaviourHasChanged();
    static Map<String, String> GetBehaviourHashesMap();
    static String GetBehaviourHash(const String &behaviourFilepath);

    static QLibrary* LoadLibraryFromFilepath(const String &libFilepath);
    static void RemoveLibraryFiles();

    friend class Application;
    friend class BehaviourHolder;
    friend class BehavioursLibCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

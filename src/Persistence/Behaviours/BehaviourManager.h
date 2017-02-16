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
#include "BehaviourManagerStatus.h"
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

    static BehaviourManager* GetInstance();

    /**
     * This must be called when you want to retrieve the QLibrary from a
     * behaviour source filepath.
    **/
    static void Load(BehaviourHolder *behaviourHolder, const String &behaviourPath);

    static void RefreshAllBehaviours();

    static const BehaviourManagerStatus &GetStatus();

public slots:
    // Called by the BehaviourCompileRunnable when has finished
    void OnBehaviourSuccessCompiling(const QString &behaviourPath,
                                      const QString &libraryFilepath,
                                      const QString &warnMessage);

    // Called by the BehaviourCompileRunnable when has failed
    void OnBehaviourFailedCompiling(const QString &behaviourPath,
                                    const QString &errorMessage);

private:
    QThreadPool m_threadPool;

    BehaviourManagerStatus m_status;
    BehaviourRefresherTimer m_behaviourRefresherTimer;

    BehaviourManager();

    static QLibrary* LoadLibraryFromFilepath(const String &libFilepath);
    static void RemoveOutdatedLibraryFiles(const String &newLibraryFilepath);

    friend class Application;
    friend class BehaviourHolder;
    friend class BehaviourCompileRunnable;
};

#endif // BEHAVIOURMANAGER_H

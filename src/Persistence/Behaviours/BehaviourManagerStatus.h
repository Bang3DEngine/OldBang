#ifndef BEHAVIOURMANAGERSTATUS_H
#define BEHAVIOURMANAGERSTATUS_H

#include <QMutex>

#include "Map.h"
#include "List.h"
#include "String.h"
#include "ListLogger.h"
#include "Persistence.h"


class BehaviourId
{
public:
    BehaviourId(const String &behAbsPath)
    {
        behaviourAbsPath = Persistence::ToAbsolute(behAbsPath, false);
        hash = Persistence::GetHash(behaviourAbsPath);
    }

    BehaviourId(const String &behAbsPath, const String &_hash)
    {
        behaviourAbsPath = Persistence::ToAbsolute(behAbsPath, false);
        hash = _hash;
    }

    String behaviourAbsPath = "";
    String hash = "";
};

class QLibrary;
class BehaviourHolder;
class BehaviourManagerStatus
{
public:
    bool IsBeingCompiled(const BehaviourId &bid) const;
    bool HasFailed(const BehaviourId &bid) const;
    bool IsCached(const BehaviourId &bid) const;

    QLibrary *GetLibrary(const BehaviourId &bid) const;

    bool SomeBehaviourWithError() const;
    float GetBehaviourHoldersUpdatedPercent() const;

private:

    mutable QMutex m_mutex;

    std::set<BehaviourId> m_beingCompiled;
    std::set<BehaviourId> m_failed;

    Map<BehaviourId, QLibrary*> m_libraries;
    Map<BehaviourId, List<BehaviourHolder*> > m_demanders;
    Map<BehaviourId, List<ListLogger::MessageId> > m_failMessages;

    BehaviourManagerStatus();

    List<BehaviourHolder*>& GetDemanders(const BehaviourId &bid);

    void TreatIfBehaviourChanged(const String &behaviourPath);

    void OnBehaviourDemanded(const String &behaviourPath,
                             BehaviourHolder *bHolder);
    void OnBehaviourStartedCompiling(const String &behaviourPath);
    void OnBehaviourFinishedCompiling(const String &behaviourPath,
                                      const String &libraryFilepath,
                                      const String &warnMessage,
                                      QLibrary *loadedLibrary);
    void OnBehaviourFailedCompiling(const String &behaviourPath,
                                    const String &errorMessage);
    void OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder);

    friend class BehaviourManager;
};

static bool operator<(BehaviourId bid0, BehaviourId bid1)
{
    int pathCompare = bid0.behaviourAbsPath.compare(bid1.behaviourAbsPath);
    if (pathCompare == 0)
    {
        return bid0.hash.compare(bid1.hash) < 0;
    }
    return pathCompare <= 0;
}

static bool operator==(BehaviourId bid0, BehaviourId bid1)
{
    return (bid0.behaviourAbsPath == bid1.behaviourAbsPath) &&
           (bid0.hash == bid1.hash);
}

#endif // BEHAVIOURMANAGERSTATUS_H

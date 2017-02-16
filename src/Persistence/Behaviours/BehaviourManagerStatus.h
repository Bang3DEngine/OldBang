#ifndef BEHAVIOURMANAGERSTATUS_H
#define BEHAVIOURMANAGERSTATUS_H

#include <QMutex>

#include "Map.h"
#include "List.h"
#include "String.h"
#include "Persistence.h"

#ifdef BANG_EDITOR
#include "ListLogger.h"
#endif

#include "IToString.h"

class BehaviourId : public IToString
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

    String ToString() const override { return behaviourAbsPath + "(" + hash + ")"; }

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
    bool IsNewOrHasChanged(const BehaviourId &bid) const;

    QLibrary *GetLibrary(const BehaviourId &bid) const;

    bool SomeBehaviourWithError() const;
    float GetBehaviourHoldersUpdatedPercent() const;

private:
    std::set<BehaviourId> m_failed;
    std::set<BehaviourId> m_beingCompiled;

    Map<BehaviourId, QLibrary*> m_libraries;

    #ifdef BANG_EDITOR
    Map<String, List<ListLogger::MessageId> > m_failMessagesIds;
    #else
    Map<String, List<int> > m_failMessagesIds; // useless map :)
    #endif

    BehaviourManagerStatus();

    void TreatIfBehaviourChanged(const String &behaviourPath);

    void OnBehaviourStartedCompiling(const String &behaviourPath);
    void OnBehaviourSuccessCompiling(const String &behaviourPath,
                                      const String &libraryFilepath,
                                      const String &warnMessage,
                                      QLibrary *loadedLibrary);
    void OnBehaviourFailedCompiling(const String &behaviourPath,
                                    const String &errorMessage);

    void ClearFails(const String &behaviourPath);

    List<BehaviourId> GetCurrentBehaviourIds() const;

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

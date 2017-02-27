#include "BehaviourManagerStatus.h"

#include <QLibrary>

#include "Scene.h"
#include "Debug.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManager.h"

#ifdef BANG_EDITOR
#include "Console.h"
#endif

BehaviourManagerStatus::BehaviourManagerStatus()
{
}

bool BehaviourManagerStatus::IsBeingCompiled(const BehaviourId &bid) const
{
    return m_beingCompiled.count(bid) > 0;
}

bool BehaviourManagerStatus::HasFailed(const BehaviourId &bid) const
{
    return m_failed.count(bid) > 0;
}

bool BehaviourManagerStatus::HasFailed(const String &behaviourFilepath) const
{
    return HasFailed( BehaviourId(behaviourFilepath) );
}

bool BehaviourManagerStatus::IsReady(const BehaviourId &bid) const
{
    return m_successfullyCompiled.count(bid) > 0 &&
           !IsBeingCompiled(bid) &&
           !HasFailed(bid);
}

bool BehaviourManagerStatus::IsReady(const String &behaviourFilepath) const
{
    return IsReady( BehaviourId(behaviourFilepath) );
}

bool BehaviourManagerStatus::SomeBehaviourWithError() const
{
    Scene *scene = SceneManager::GetActiveScene();
    List<BehaviourHolder*> behHolders =
            scene->GetComponentsInChildren<BehaviourHolder>();
    for (BehaviourHolder *bh : behHolders)
    {
        BehaviourId bid( bh->GetSourceFilepath() );
        if ( HasFailed(bid) )
        {
            return true;
        }
    }
    return false;
}

float BehaviourManagerStatus::GetBehaviourHoldersUpdatedPercent() const
{
    // Before calling this function RefreshAllBehaviours must be called,
    // in order for them to start compiling

    int updatedBehaviours = 0;
    int totalBehaviours = 0;
    List<String> behaviourFiles = Persistence::GetFiles(
                Persistence::GetProjectAssetsRootAbs(), true, {"cpp"});
    for (String behaviourPath : behaviourFiles)
    {
        ++totalBehaviours;

        BehaviourId bid(behaviourPath);
        bool updated = !IsBeingCompiled(bid) && !HasFailed(bid);
        if (updated) { ++updatedBehaviours; }
    }
    return float(updatedBehaviours) / totalBehaviours;
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(
        const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);
    m_beingCompiled.insert(bid);
}

void BehaviourManagerStatus::OnBehaviourSuccessCompiling(const String &behPath)
{
    BehaviourId bid(behPath);
    ClearFails(bid.behaviourAbsPath);

    m_successfullyCompiled.insert(bid);
    m_beingCompiled.erase(bid);
    #ifdef BANG_EDITOR
    m_failMessagesIds.Remove(bid.behaviourAbsPath);
    #endif

    Debug_Log("  m_successfullyCompiled: " << m_successfullyCompiled);
}

void BehaviourManagerStatus::OnBehaviourFailedCompiling(
                                                const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);

    m_beingCompiled.erase(bid);
    m_failed.insert(bid);
}

void BehaviourManagerStatus::ClearFails(const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);

    // Remove from m_failed all the bids with the same path.
    List<BehaviourId> currentBids = GetCurrentBehaviourIds();
    for (const BehaviourId &oldBid : currentBids)
    {
        if (oldBid.behaviourAbsPath == bid.behaviourAbsPath)
        {
            m_failed.erase(oldBid);
        }
    }

    // Clear the fail messages from the Console.
    #ifdef BANG_EDITOR
    for (Console::MessageId msgId
                : m_failMessagesIds.Get(bid.behaviourAbsPath))
    {
        Console::GetInstance()->ClearMessage(msgId);
    }
    m_failMessagesIds.Remove(bid.behaviourAbsPath);
    #endif
}

List<BehaviourId> BehaviourManagerStatus::GetCurrentBehaviourIds() const
{
    List<BehaviourId> compiled;
    for (const BehaviourId &x : m_successfullyCompiled) { compiled.Add(x); }
    List<BehaviourId> failed;
    for (const BehaviourId &x : m_failed) { failed.Add(x); }
    compiled.Splice(compiled.End(), failed);
    return compiled;
}


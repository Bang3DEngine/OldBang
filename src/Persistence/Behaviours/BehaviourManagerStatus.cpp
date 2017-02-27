#include "BehaviourManagerStatus.h"

#include <QLibrary>

#include "Scene.h"
#include "Debug.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"

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

bool BehaviourManagerStatus::IsCached(const BehaviourId &bid) const
{
    bool beingCompiled = IsBeingCompiled(bid);
    return m_libraries.ContainsKey(bid) && !beingCompiled;
}

bool BehaviourManagerStatus::IsNewOrHasChanged(const BehaviourId &bid) const
{
    return (!IsCached(bid) && !HasFailed(bid));
}

QLibrary *BehaviourManagerStatus::GetLibrary(const BehaviourId &bid) const
{
    if (m_libraries.ContainsKey(bid))
    {
        return m_libraries.Get(bid);
    }
    return nullptr;
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

void BehaviourManagerStatus::TreatIfBehaviourChanged(const String &behPath)
{
    BehaviourId newBid(behPath);
    if ( IsBeingCompiled(newBid) ) { return; }

    bool behaviourHasChanged = IsNewOrHasChanged(newBid);

    if (behaviourHasChanged)
    {
        // Clear error messages, if there's any error, it will be
        // produced again when compiling
        ClearFails(newBid.behaviourAbsPath);

        const List<BehaviourId> &currentBids = GetCurrentBehaviourIds();
        for (const BehaviourId &possiblyOutdatedBid : currentBids)
        {
            if (possiblyOutdatedBid.behaviourAbsPath == newBid.behaviourAbsPath)
            {
                // Remove the outdated references and old cached libraries
                m_failed.erase(possiblyOutdatedBid);
                m_libraries.Remove(possiblyOutdatedBid);
                m_beingCompiled.erase(possiblyOutdatedBid);
                #ifdef BANG_EDITOR
                m_failMessagesIds.Remove(behPath);
                #endif
            }
        }
    }
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(
        const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);
    m_beingCompiled.insert(bid);
}

void BehaviourManagerStatus::OnBehaviourSuccessCompiling(
                                                const String &behaviourPath,
                                                const String &libraryFilepath,
                                                const String &warnMessage,
                                                QLibrary *loadedLibrary)
{
    BehaviourId bid(behaviourPath);
    ClearFails(bid.behaviourAbsPath);

    m_libraries.Set(bid, loadedLibrary);
    m_beingCompiled.erase(bid);
    #ifdef BANG_EDITOR
    m_failMessagesIds.Remove(bid.behaviourAbsPath);
    #endif
}

void BehaviourManagerStatus::OnBehaviourFailedCompiling(
                                                const String &behaviourPath,
                                                const String &errorMessage)
{
    BehaviourId bid(behaviourPath);

    // Clear old fails of the same behaviour (but with outdated code)
    ClearFails(bid.behaviourAbsPath);

    #ifdef BANG_EDITOR
    if (!errorMessage.Empty())
    {
        Console *lLog = Console::GetInstance();
        Console::MessageId msgId = lLog->AddError(errorMessage, 1,
                                                     behaviourPath, true);
        m_failMessagesIds.Get(behaviourPath).PushBack(msgId);
    }
    #endif

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
    List<BehaviourId> compiled = m_libraries.GetKeys();
    List<BehaviourId> failed;
    for (const BehaviourId &x : m_failed) { failed.Add(x); }
    compiled.Splice(compiled.End(), failed);
    return compiled;
}


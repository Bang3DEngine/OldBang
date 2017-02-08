#include "BehaviourManagerStatus.h"

#include <QLibrary>

#include "Scene.h"
#include "Debug.h"
#include "ListLogger.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"

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

List<BehaviourHolder*> &BehaviourManagerStatus::
            GetDemanders(const BehaviourId &bid)
{
    return m_demanders.Get(bid);
}

bool BehaviourManagerStatus::SomeBehaviourWithError() const
{
    Scene *scene = SceneManager::GetActiveScene();
    List<BehaviourHolder*> behHolders = scene->GetComponentsInChildren<BehaviourHolder>();
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
    Scene *scene = SceneManager::GetActiveScene();

    int behHoldersUpdated = 0;
    int totalBehaviourHolders = 0;
    List<BehaviourHolder*> behHolders = scene->GetComponentsInChildren<BehaviourHolder>();
    for (BehaviourHolder *bh : behHolders)
    {
        String behaviourPath = bh->GetSourceFilepath();
        if (behaviourPath.Empty()) { continue; }

        ++totalBehaviourHolders;

        BehaviourId bid(behaviourPath);
        QLibrary *updatedLibrary = GetLibrary(bid);
        if (!updatedLibrary) { continue; } // Not even compiled

        QLibrary *currentBHLibrary = bh->GetLibraryBeingUsed();
        bool updated = (currentBHLibrary == updatedLibrary);
        if (updated) { ++behHoldersUpdated; }
    }

    return float(behHoldersUpdated) / totalBehaviourHolders;
}

void BehaviourManagerStatus::TreatIfBehaviourChanged(const String &behaviourPath)
{
    BehaviourId newBid(behaviourPath);
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
                m_demanders.Remove(possiblyOutdatedBid);
                m_libraries.Remove(possiblyOutdatedBid);
                m_beingCompiled.erase(possiblyOutdatedBid);
                m_failMessagesIds.Remove(behaviourPath);
            }
        }
    }
}

void BehaviourManagerStatus::OnBehaviourDemanded(const String &behaviourPath,
                                                 BehaviourHolder *bHolder)
{
    BehaviourId bid(behaviourPath);
    List<BehaviourHolder*> &demanders = GetDemanders(bid);

    if (!demanders.Contains(bHolder))
    {
        demanders.PushBack(bHolder);
    }
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);
    m_beingCompiled.insert(bid);
}

void BehaviourManagerStatus::OnBehaviourSuccessCompiling(const String &behaviourPath,
                                                         const String &libraryFilepath,
                                                         const String &warnMessage,
                                                         QLibrary *loadedLibrary)
{
    BehaviourId bid(behaviourPath);
    ClearFails(bid.behaviourAbsPath);

    m_libraries.Set(bid, loadedLibrary);
    m_beingCompiled.erase(bid);
    m_demanders.Remove(bid);
    m_failMessagesIds.Remove(bid.behaviourAbsPath);
}

void BehaviourManagerStatus::OnBehaviourFailedCompiling(const String &behaviourPath,
                                                        const String &errorMessage)
{
    BehaviourId bid(behaviourPath);

    // Clear old fails of the same behaviour (but with outdated code)
    ClearFails(bid.behaviourAbsPath);

    if (!errorMessage.Empty())
    {
        ListLogger *lLog = ListLogger::GetInstance();
        ListLogger::MessageId msgId = lLog->AddError(errorMessage, 1,
                                                     behaviourPath, true);
        m_failMessagesIds.Get(behaviourPath).PushBack(msgId);
    }
    m_beingCompiled.erase(bid);
    m_failed.insert(bid);
}

void BehaviourManagerStatus::OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    // Erase the behaviourHolder from all the demand lists it is in
    BehaviourId bid(behaviourHolder->GetSourceFilepath());
    List<BehaviourHolder*> demanders = GetDemanders(bid);

    demanders.RemoveAll(behaviourHolder);
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

    // Clear the fail messages from the logger.
    for (ListLogger::MessageId msgId : m_failMessagesIds.Get(bid.behaviourAbsPath))
    {
        ListLogger::GetInstance()->ClearMessage(msgId);
    }
    m_failMessagesIds.Remove(bid.behaviourAbsPath);
}

List<BehaviourId> BehaviourManagerStatus::GetCurrentBehaviourIds() const
{
    List<BehaviourId> compiled = m_libraries.GetKeys();
    List<BehaviourId> failed;
    for (const BehaviourId &x : m_failed) { failed.Add(x); }
    compiled.Splice(compiled.End(), failed);
    return compiled;
}


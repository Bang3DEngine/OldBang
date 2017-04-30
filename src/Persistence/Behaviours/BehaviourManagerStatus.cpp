#include "Bang/BehaviourManagerStatus.h"

#include <QLibrary>
#include "Bang/WinUndef.h"

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Behaviour.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"

#ifdef BANG_EDITOR
#include "Bang/Console.h"
#endif

BehaviourManagerStatus::BehaviourManagerStatus()
{
}

bool BehaviourManagerStatus::AllBehavioursReady() const
{
    return GetPercentOfReadyBehaviours() >= 1.0f;
}

bool BehaviourManagerStatus::AllBehavioursReadyOrFailed() const
{
    List<String> sourcesFilepaths =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    for (const String &srcFilepath : sourcesFilepaths)
    {
        if (!IsReady(srcFilepath) && !HasFailed(srcFilepath))
        {
            return false;
        }
    }
    return true;
}

float BehaviourManagerStatus::GetPercentOfReadyBehaviours() const
{
    List<String> sourcesFilepaths =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    if (sourcesFilepaths.Empty()) { return 1.0f; }

    int compiledBehaviours = 0;
    for (const String &srcFilepath : sourcesFilepaths)
    {
        if (IsReady(srcFilepath)) { ++compiledBehaviours; }
    }
    return float(compiledBehaviours) / sourcesFilepaths.Size();
}

bool BehaviourManagerStatus::IsBeingCompiled(const BehaviourId &bid) const
{
    return m_beingCompiled.Contains(bid);
}

bool BehaviourManagerStatus::HasFailed(const BehaviourId &bid) const
{
    return m_failed.Contains(bid);
}

bool BehaviourManagerStatus::HasFailed(const String &behaviourFilepath) const
{
    return HasFailed( BehaviourId(behaviourFilepath) );
}

bool BehaviourManagerStatus::IsReady(const BehaviourId &bid) const
{
    String behaviourName = IO::GetBaseName(bid.behaviourAbsPath);
    String behaviourObjectFilepath =
            BehaviourManager::GetCurrentLibsDir() + "/" + behaviourName + ".o";
    return m_successfullyCompiled.Contains(bid) &&
           !IsBeingCompiled(bid) &&
           !HasFailed(bid) &&
           IO::Exists(behaviourObjectFilepath);
}

bool BehaviourManagerStatus::IsReady(const String &behaviourFilepath) const
{
    return IsReady( BehaviourId(behaviourFilepath) );
}

bool BehaviourManagerStatus::IsBehavioursLibraryReady() const
{
    return m_behavioursLibraryReady;
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(
        const String &behaviourPath)
{
    m_behavioursLibraryReady = false;
    BehaviourId bid(behaviourPath);
    m_beingCompiled.Insert(bid);
    ClearFails(bid.behaviourAbsPath);
}

void BehaviourManagerStatus::OnBehaviourSuccessCompiling(const String &behPath)
{
    BehaviourId bid(behPath);
    m_successfullyCompiled.Insert(bid);
    m_beingCompiled.Remove(bid);
    ClearFails(bid.behaviourAbsPath);
}

void BehaviourManagerStatus::OnBehaviourFailedCompiling(
        const String &behaviourPath, const String &errorMessage)
{
    ClearFails(behaviourPath);
    BehaviourId bid(behaviourPath);
    m_beingCompiled.Remove(bid);
    m_failed.Insert(bid);

    #ifdef BANG_EDITOR
    Console::MessageId failMsgId =
            Console::AddError(errorMessage, __LINE__, __FILE__, true);
    m_failMessagesIds[behaviourPath].PushBack(failMsgId);
    #endif
}

void BehaviourManagerStatus::OnBehavioursLibraryReady()
{
    m_behavioursLibraryReady = true;
}

void BehaviourManagerStatus::InvalidateBehavioursLibraryReady()
{
    m_behavioursLibraryReady = false;
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
            m_failed.Remove(oldBid);
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


bool operator<(BehaviourId bid0, BehaviourId bid1)
{
    int pathCompare = bid0.behaviourAbsPath.compare(bid1.behaviourAbsPath);
    if (pathCompare == 0)
    {
        return bid0.hash.compare(bid1.hash) < 0;
    }
    return pathCompare <= 0;
}

bool operator==(BehaviourId bid0, BehaviourId bid1)
{
    return (bid0.behaviourAbsPath == bid1.behaviourAbsPath) &&
            (bid0.hash == bid1.hash);
}

#include "Bang/BehaviourManagerStatus.h"

#include <QLibrary>

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Behaviour.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"

BehaviourManagerStatus::BehaviourManagerStatus()
{
}

bool BehaviourManagerStatus::AllBehavioursReady(const Path &libsDir) const
{
    return GetPercentOfReadyBehaviours(libsDir) >= 1.0f;
}

bool BehaviourManagerStatus::AllBehavioursReadyOrFailed(const Path &libsDir) const
{
    List<Path> srcs = Paths::GetBehavioursSourcesFilepaths();
    return srcs.All( BPRED( this->IsReady(x, libsDir) || this->HasFailed(x) ) );
}

float BehaviourManagerStatus::GetPercentOfReadyBehaviours(const Path &libsDir) const
{
    List<Path> sourcesFilepaths = Paths::GetBehavioursSourcesFilepaths();
    if (sourcesFilepaths.IsEmpty()) { return 1.0f; }

    uint compiledBehaviours =
            sourcesFilepaths.Count( BPRED(this->IsReady(x, libsDir)) );
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

bool BehaviourManagerStatus::HasFailed(const Path &behaviourFilepath) const
{
    return HasFailed( BehaviourId(behaviourFilepath) );
}

bool BehaviourManagerStatus::IsReady(const BehaviourId &bid,
                                     const Path &libsDir) const
{
    String behaviourName = bid.behaviourPath.GetName();
    Path behaviourObjectFilepath = libsDir.Append(behaviourName)
                                          .AppendExtension("o");

    bool ready = !IsBeingCompiled(bid) &&
                 !HasFailed(bid) &&
                 behaviourObjectFilepath.Exists();
    return ready;
}

bool BehaviourManagerStatus::IsReady(const Path &behaviourFilepath,
                                     const Path &libsDir) const
{
    return IsReady( BehaviourId(behaviourFilepath), libsDir );
}

bool BehaviourManagerStatus::IsBehavioursLibraryReady() const
{
    return m_behavioursLibraryReady;
}

bool BehaviourManagerStatus::HasMergingSucceed() const
{
    return m_mergeState == MergingState::Success;
}

bool BehaviourManagerStatus::IsMerging() const
{
    return m_mergeState == MergingState::Merging;
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(
        const Path &behaviourPath)
{
    m_behavioursLibraryReady = false;
    BehaviourId bid(behaviourPath);
    m_beingCompiled.Insert(bid);
    ClearFails(bid.behaviourPath);
}

void BehaviourManagerStatus::OnBehaviourSuccessCompiling(const Path &behPath)
{
    BehaviourId bid(behPath);
    m_successfullyCompiled.Insert(bid);
    m_beingCompiled.Remove(bid);
    ClearFails(bid.behaviourPath);
}

void BehaviourManagerStatus::OnBehaviourObjectCompilationFailed(
        const Path &behaviourPath,
        const String &errorMessage)
{
    ClearFails(behaviourPath);
    BehaviourId bid(behaviourPath);
    m_beingCompiled.Remove(bid);
    m_failed.Insert(bid);
}

void BehaviourManagerStatus::OnMergingStarted()
{
    m_mergeState = MergingState::Merging;
}

void BehaviourManagerStatus::OnMergingFinished(bool success)
{
    m_mergeState = success ? MergingState::Success : MergingState::Failed;
}

void BehaviourManagerStatus::OnBehavioursLibraryReady()
{
    m_behavioursLibraryReady = true;
}

void BehaviourManagerStatus::InvalidateBehavioursLibraryReady()
{
    m_behavioursLibraryReady = false;
}

void BehaviourManagerStatus::ClearFails(const Path &behaviourPath)
{
    BehaviourId bid(behaviourPath);

    // Remove from m_failed all the bids with the same path.
    List<BehaviourId> currentBids = GetCurrentBehaviourIds();
    for (const BehaviourId &oldBid : currentBids)
    {
        if (oldBid.behaviourPath == bid.behaviourPath)
        {
            m_failed.Remove(oldBid);
        }
    }
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
    int pathCompare = bid0.behaviourPath.GetAbsolute().compare(
                                    bid1.behaviourPath.GetAbsolute());
    if (pathCompare == 0)
    {
        return bid0.hash.compare(bid1.hash) < 0;
    }
    return pathCompare <= 0;
}

bool operator==(BehaviourId bid0, BehaviourId bid1)
{
    return (bid0.behaviourPath == bid1.behaviourPath) &&
            (bid0.hash == bid1.hash);
}

BehaviourId::BehaviourId(const Path &behPath)
{
    behaviourPath = behPath;
    String code = File::GetContents(behaviourPath);

    List<Path> includePaths = Paths::ProjectAssets().GetSubDirectories(true);
    includePaths.Add(Paths::ProjectAssets());
    CodePreprocessor::PreprocessCode(&code, includePaths);
    hash = File::GetHashFromString(code);
}

BehaviourId::BehaviourId(const Path &behPath, const String &_hash)
{
    behaviourPath = behPath;
    hash = _hash;
}

String BehaviourId::ToString() const
{
    return behaviourPath + "(" + hash + ")";
}

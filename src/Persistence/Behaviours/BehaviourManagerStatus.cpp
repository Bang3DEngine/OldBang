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
    QMutexLocker locker(&m_mutex);
    return m_beingCompiled.count(bid) > 0;
}

bool BehaviourManagerStatus::HasFailed(const BehaviourId &bid) const
{
    QMutexLocker locker(&m_mutex);
    return m_failed.count(bid) > 0;
}

bool BehaviourManagerStatus::IsCached(const BehaviourId &bid) const
{
    bool beingCompiled = IsBeingCompiled(bid);
    QMutexLocker locker(&m_mutex);
    return m_libraries.ContainsKey(bid) && !beingCompiled;
}

QLibrary *BehaviourManagerStatus::GetLibrary(const BehaviourId &bid) const
{
    QMutexLocker locker(&m_mutex);
    if (m_libraries.ContainsKey(bid))
    {
        return m_libraries.Get(bid);
    }
    return nullptr;
}

List<BehaviourHolder*> &BehaviourManagerStatus::
            GetDemanders(const BehaviourId &bid)
{
    QMutexLocker locker(&m_mutex);
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
    int behHoldersUpdated = 0;
    Scene *scene = SceneManager::GetActiveScene();
    List<BehaviourHolder*> behHolders = scene->GetComponentsInChildren<BehaviourHolder>();
    for (BehaviourHolder *bh : behHolders)
    {
        BehaviourId bid( bh->GetSourceFilepath() );
        QLibrary *updatedLibrary = GetLibrary(bid);
        if (!updatedLibrary) { continue; } // Not even compiled

        QLibrary *currentBHLibrary = bh->GetLibraryBeingUsed();
        bool updated = (currentBHLibrary == updatedLibrary);
        if (updated) { ++behHoldersUpdated; }
    }

    return float(behHoldersUpdated) / behHolders.Size();
}

void BehaviourManagerStatus::TreatIfBehaviourChanged(const String &behaviourPath)
{
    BehaviourId bid(behaviourPath);
    bool behaviourHasChanged = !IsCached(bid);

    QMutexLocker locker(&m_mutex);
    if (behaviourHasChanged)
    {
        // Remove the outdated references
        m_failed.erase(bid);
        m_demanders.Remove(bid);
        m_libraries.Remove(bid);
        m_beingCompiled.erase(bid);
        m_failMessages.Remove(bid);
    }
}

void BehaviourManagerStatus::OnBehaviourDemanded(const String &behaviourPath,
                                                 BehaviourHolder *bHolder)
{
    Debug_Log("Behaviour demanded " << behaviourPath);
    BehaviourId bid(behaviourPath);
    List<BehaviourHolder*> &demanders = GetDemanders(bid);

    QMutexLocker locker(&m_mutex);
    if (!demanders.Contains(bHolder))
    {
        demanders.PushBack(bHolder);
    }
}

void BehaviourManagerStatus::OnBehaviourStartedCompiling(const String &behaviourPath)
{
    QMutexLocker locker(&m_mutex);
    Debug_Log("Behaviour started compiling " << behaviourPath);
    BehaviourId bid(behaviourPath);
    m_beingCompiled.insert(bid);
    String str = "";
    for (BehaviourId ibid : m_beingCompiled)
    {
        str += "(" + ibid.behaviourAbsPath + ", " + ibid.hash + "),  ";
    }
    Debug_Log(str);
}

void BehaviourManagerStatus::OnBehaviourFinishedCompiling(const String &behaviourPath,
                                                          const String &libraryFilepath,
                                                          const String &warnMessage,
                                                          QLibrary *loadedLibrary)
{
    Debug_Log("Behaviour finished compiling " << behaviourPath);
    BehaviourId bid(behaviourPath);

    QMutexLocker locker(&m_mutex);
    m_libraries.Set(bid, loadedLibrary);
    m_beingCompiled.erase(bid);
    m_failMessages.Remove(bid);
    m_demanders.Remove(bid);
}

void BehaviourManagerStatus::OnBehaviourFailedCompiling(const String &behaviourPath,
                                                        const String &errorMessage)
{
    Debug_Log("Behaviour failed compiling " << behaviourPath);
    BehaviourId bid(behaviourPath);

    QMutexLocker locker(&m_mutex);
    if (!errorMessage.Empty())
    {
        ListLogger *lLog = ListLogger::GetInstance();
        ListLogger::MessageId msgId = lLog->AddError(errorMessage, 1,
                                                     behaviourPath, true);
        Debug_Log("Adding error message with id: " << msgId);
        m_failMessages[bid].PushBack(msgId);
    }
    m_beingCompiled.erase(bid);
    m_failed.insert(bid);
}

void BehaviourManagerStatus::OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    // Erase the behaviourHolder from all the demand lists it is in
    BehaviourId bid(behaviourHolder->GetSourceFilepath());
    List<BehaviourHolder*> demanders = GetDemanders(bid);

    QMutexLocker locker(&m_mutex);
    demanders.RemoveAll(behaviourHolder);
}


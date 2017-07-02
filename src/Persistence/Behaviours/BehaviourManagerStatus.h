#ifndef BEHAVIOURMANAGERSTATUS_H
#define BEHAVIOURMANAGERSTATUS_H

#include <QMutex>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Set.h"
#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/CodePreprocessor.h"

#ifdef BANG_EDITOR
#include "Bang/Console.h"
#endif

#include "Bang/IToString.h"

class QLibrary;
class Behaviour;
class BehaviourId;
class BehaviourManagerStatus
{
public:
    bool AllBehavioursReady(const Path &libsDir) const;
    bool AllBehavioursReadyOrFailed(const Path &libsDir) const;
    float GetPercentOfReadyBehaviours(const Path &libsDir) const;
    bool IsBeingCompiled(const BehaviourId &bid) const;
    bool HasFailed(const BehaviourId &bid) const;
    bool HasFailed(const Path &behaviourFilepath) const;
    bool IsReady(const BehaviourId &bid,
                 const Path &libsDir = Paths::ProjectLibrariesDir()) const;
    bool IsReady(const Path &behaviourFilepath,
                 const Path &libsDir = Paths::ProjectLibrariesDir()) const;

    bool IsBehavioursLibraryReady() const;
    bool HasMergingSucceed() const;
    bool IsMerging() const;

private:
    enum MergingState { Idle, Merging, Success, Failed };
    MergingState m_mergeState = MergingState::Idle;
    bool m_behavioursLibraryReady = false;

    Set<BehaviourId> m_failed;
    Set<BehaviourId> m_beingCompiled;
    Set<BehaviourId> m_successfullyCompiled;

    #ifdef BANG_EDITOR
    Map<Path, List<Console::MessageId> > m_failMessagesIds;
    #else
    Map<String, List<int> > m_failMessagesIds; // useless map :)
    #endif

    BehaviourManagerStatus();

    void OnBehaviourStartedCompiling(const Path &behaviourPath);
    void OnBehaviourSuccessCompiling(const Path &behaviourPath);
    void OnBehaviourObjectCompilationFailed(const Path &behaviourPath,
                                    const String &errorMessage);
    void OnMergingStarted();
    void OnMergingFinished(bool success);

    void OnBehavioursLibraryReady();
    void InvalidateBehavioursLibraryReady();

    void ClearFails(const Path &behaviourPath);

    List<BehaviourId> GetCurrentBehaviourIds() const;

    friend class BehaviourManager;
};

class BehaviourId : public IToString
{
public:
    Path behaviourPath;
    String hash = "";

    BehaviourId(const Path &behPath);
    BehaviourId(const Path &behPath, const String &_hash);
    String ToString() const override;
};

bool operator<(BehaviourId bid0, BehaviourId bid1);
bool operator==(BehaviourId bid0, BehaviourId bid1);

#endif // BEHAVIOURMANAGERSTATUS_H

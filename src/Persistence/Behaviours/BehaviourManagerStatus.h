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

class BehaviourId : public IToString
{
public:
    Path behaviourPath;
    String hash = "";

    BehaviourId(const Path &behPath)
    {
        behaviourPath = behPath;
        String code = File::GetContents(behaviourPath);

        List<Path> includePaths = Paths::ProjectAssets().GetSubDirectories(true);
        includePaths.Add(Paths::ProjectAssets());
        CodePreprocessor::PreprocessCode(&code, includePaths);
        hash = File::GetHashFromString(code);
    }

    BehaviourId(const Path &behPath, const String &_hash)
    {
        behaviourPath = behPath;
        hash = _hash;
    }

    String ToString() const override
    {
        return behaviourPath + "(" + hash + ")";
    }
};

class QLibrary;
class Behaviour;
class BehaviourManagerStatus
{
public:
    bool AllBehavioursReady() const;
    bool AllBehavioursReadyOrFailed() const;
    float GetPercentOfReadyBehaviours() const;
    bool IsBeingCompiled(const BehaviourId &bid) const;
    bool HasFailed(const BehaviourId &bid) const;
    bool HasFailed(const Path &behaviourFilepath) const;
    bool IsReady(const BehaviourId &bid) const;
    bool IsReady(const Path &behaviourFilepath) const;

    bool IsBehavioursLibraryReady() const;

private:
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
    void OnBehaviourFailedCompiling(const Path &behaviourPath,
                                    const String &errorMessage);
    void OnBehavioursLibraryReady();
    void InvalidateBehavioursLibraryReady();

    void ClearFails(const Path &behaviourPath);

    List<BehaviourId> GetCurrentBehaviourIds() const;

    friend class BehaviourManager;
};

bool operator<(BehaviourId bid0, BehaviourId bid1);
bool operator==(BehaviourId bid0, BehaviourId bid1);

#endif // BEHAVIOURMANAGERSTATUS_H

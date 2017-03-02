#ifndef BEHAVIOURMANAGERSTATUS_H
#define BEHAVIOURMANAGERSTATUS_H

#include <QMutex>

#include "Map.h"
#include "List.h"
#include "File.h"
#include "String.h"
#include "Persistence.h"
#include "CodePreprocessor.h"

#ifdef BANG_EDITOR
#include "Console.h"
#endif

#include "IToString.h"

class BehaviourId : public IToString
{
public:
    String behaviourAbsPath = "";
    String hash = "";

    BehaviourId(const String &behAbsPath)
    {
        behaviourAbsPath = Persistence::ToAbsolute(behAbsPath, false);
        String code = File::GetContents(behaviourAbsPath);

        List<String> includePaths = Persistence::GetSubDirectories(
                    Persistence::GetProjectAssetsRootAbs(), true);
        CodePreprocessor::PreprocessCode(&code, includePaths);

        hash = Persistence::GetHashFromString(code);
    }

    BehaviourId(const String &behAbsPath, const String &_hash)
    {
        behaviourAbsPath = Persistence::ToAbsolute(behAbsPath, false);
        hash = _hash;
    }

    String ToString() const override
    {
        return behaviourAbsPath + "(" + hash + ")";
    }
};

class QLibrary;
class BehaviourHolder;
class BehaviourManagerStatus
{
public:
    bool AllBehavioursReady() const;
    bool AllBehavioursReadyOrFailed() const;
    float GetPercentOfReadyBehaviours() const;
    bool IsBeingCompiled(const BehaviourId &bid) const;
    bool HasFailed(const BehaviourId &bid) const;
    bool HasFailed(const String &behaviourFilepath) const;
    bool IsReady(const BehaviourId &bid) const;
    bool IsReady(const String &behaviourFilepath) const;

    bool IsBehavioursLibraryReady() const;

private:
    bool m_behavioursLibraryReady = false;

    std::set<BehaviourId> m_failed;
    std::set<BehaviourId> m_beingCompiled;
    std::set<BehaviourId> m_successfullyCompiled;

    #ifdef BANG_EDITOR
    Map<String, List<Console::MessageId> > m_failMessagesIds;
    #else
    Map<String, List<int> > m_failMessagesIds; // useless map :)
    #endif

    BehaviourManagerStatus();

    void OnBehaviourStartedCompiling(const String &behaviourPath);
    void OnBehaviourSuccessCompiling(const String &behaviourPath);
    void OnBehaviourFailedCompiling(const String &behaviourPath,
                                    const String &errorMessage);
    void OnBehavioursLibraryReady();

    void ClearFails(const String &behaviourPath);

    List<BehaviourId> GetCurrentBehaviourIds() const;

    friend class BehaviourManager;
};

bool operator<(BehaviourId bid0, BehaviourId bid1);
bool operator==(BehaviourId bid0, BehaviourId bid1);

#endif // BEHAVIOURMANAGERSTATUS_H

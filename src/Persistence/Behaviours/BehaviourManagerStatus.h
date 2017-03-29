#ifndef BEHAVIOURMANAGERSTATUS_H
#define BEHAVIOURMANAGERSTATUS_H

#include <QMutex>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Set.h"
#include "Bang/Map.h"
#include "Bang/List.h"
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
    String behaviourAbsPath = "";
    String hash = "";

    BehaviourId(const String &behAbsPath)
    {
        behaviourAbsPath = IO::ToAbsolute(behAbsPath, false);
        String code = File::GetContents(behaviourAbsPath);

        List<String> includePaths = IO::GetSubDirectories(
                    IO::GetProjectAssetsRootAbs(), true);
        CodePreprocessor::PreprocessCode(&code, includePaths);

        hash = IO::GetHashFromString(code);
    }

    BehaviourId(const String &behAbsPath, const String &_hash)
    {
        behaviourAbsPath = IO::ToAbsolute(behAbsPath, false);
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

    Set<BehaviourId> m_failed;
    Set<BehaviourId> m_beingCompiled;
    Set<BehaviourId> m_successfullyCompiled;

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

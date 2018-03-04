#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Library;
FORWARD class Behaviour;

class BehaviourManager
{
public:
	BehaviourManager();
	virtual ~BehaviourManager();

    static Behaviour* CreateBehaviourInstance(const String &behaviourName,
                                              Library *behavioursLibrary);
    static bool DeleteBehaviourInstance(const String &behaviourName,
                                        Behaviour *behaviour,
                                        Library *behavioursLibrary);

    virtual void Update();

    void SetBehavioursLibrary(const Path &libPath);
    void SetBehavioursLibrary(Library *behavioursLibrary);

    virtual bool IsInstanceCreationAllowed() const;
    Library* GetBehavioursLibrary() const;

    static BehaviourManager* GetActive();

private:
    Library *m_behavioursLibrary = nullptr;

};

NAMESPACE_BANG_END

#endif // BEHAVIOURMANAGER_H


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
};

NAMESPACE_BANG_END

#endif // BEHAVIOURMANAGER_H


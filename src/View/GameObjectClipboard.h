#ifndef COPYPASTEGAMEOBJECTMANAGER_H
#define COPYPASTEGAMEOBJECTMANAGER_H

#include <map>
#include <list>
#include "GameObject.h"

class GameObjectClipboard
{
private:
    GameObjectClipboard() {}

private:

    static std::list<GameObject*> s_copiedGameObjects;
    static std::map<GameObject*, GameObject*> s_goCopyToItsParent;

public:

    static bool HasSomethingCopied();
    static void CopyGameObjects(const std::list<GameObject*> &whatToCopy);
    static void PasteCopiedGameObjectsInto(GameObject *parent);
    static void DuplicateCopiedGameObjects();
};

#endif // COPYPASTEGAMEOBJECTMANAGER_H
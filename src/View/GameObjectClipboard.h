#ifndef COPYPASTEGAMEOBJECTMANAGER_H
#define COPYPASTEGAMEOBJECTMANAGER_H

#include "Bang/Map.h"
#include "Bang/List.h"

class GameObject;
class GameObjectClipboard
{
private:
    GameObjectClipboard() {}

private:

    static List<GameObject*> s_copiedGameObjects;
    static Map<GameObject*, GameObject*> s_goCopyToItsParent;

public:

    static bool HasSomethingCopied();
    static void CopyGameObjects(const List<GameObject*> &whatToCopy);
    static List<GameObject*> PasteCopiedGameObjectsInto(GameObject *parent);
    static List<GameObject*> DuplicateCopiedGameObjects();
};

#endif // COPYPASTEGAMEOBJECTMANAGER_H

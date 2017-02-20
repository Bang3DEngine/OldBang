#include "GameObjectClipboard.h"

#include "GameObject.h"

List<GameObject*> GameObjectClipboard::s_copiedGameObjects;
Map<GameObject*, GameObject*> GameObjectClipboard::s_goCopyToItsParent;

bool GameObjectClipboard::HasSomethingCopied()
{
    return !s_copiedGameObjects.Empty();
}

void GameObjectClipboard::CopyGameObjects(const List<GameObject*> &whatToCopy)
{
    s_copiedGameObjects.Clear();
    s_goCopyToItsParent.Clear();

    for (GameObject *go : whatToCopy)
    {
        GameObject *copy = Object::SCast<GameObject>(go->Clone());
        s_goCopyToItsParent[copy] = go->parent;
        s_copiedGameObjects.PushBack(copy);
    }

    for (GameObject *copy : s_copiedGameObjects)
    {
        copy->SetParent(nullptr);
    }
}

List<GameObject*> GameObjectClipboard::PasteCopiedGameObjectsInto(GameObject *parent)
{
    List<GameObject*> pasted;

    // We first store all the copies of the copies into a local list,
    // to avoid modifying the original copy which can be pasted multiple times.
    List<GameObject*> localCopies;
    for (GameObject *copy : s_copiedGameObjects)
    {
        GameObject *localCopy = Object::SCast<GameObject>(copy->Clone());
        localCopies.PushBack(localCopy);
    }

    for (GameObject *localCopy : localCopies)
    {
        GameObject *paste = Object::SCast<GameObject>(localCopy);
        paste->SetParent(parent);
        pasted.PushBack(paste);
    }
    return pasted;
}

List<GameObject*> GameObjectClipboard::DuplicateCopiedGameObjects()
{
    List<GameObject*> duplicated;

    for (GameObject *copy : s_copiedGameObjects)
    {
        GameObject *localCopy = Object::SCast<GameObject>(copy->Clone());
        localCopy->SetParent(s_goCopyToItsParent[copy]);
        duplicated.PushBack(localCopy);
    }
    return duplicated;
}

#include "CopyPasteGameObjectManager.h"

std::list<GameObject*> CopyPasteGameObjectManager::s_copiedGameObjects;
std::map<GameObject*, GameObject*> CopyPasteGameObjectManager::s_goCopyToItsParent;

bool CopyPasteGameObjectManager::HasSomethingCopied()
{
    return s_copiedGameObjects.size() > 0;
}

void CopyPasteGameObjectManager::CopyGameObjects(const std::list<GameObject*> &whatToCopy)
{
    s_copiedGameObjects.clear();
    s_goCopyToItsParent.clear();

    for (GameObject *go : whatToCopy)
    {
        GameObject *copy = static_cast<GameObject*>(go->Clone());
        s_goCopyToItsParent[copy] = go->parent;
        s_copiedGameObjects.push_back(copy);
    }

    for (GameObject *copy : s_copiedGameObjects)
    {
        copy->SetParent(nullptr);
    }
}

void CopyPasteGameObjectManager::PasteCopiedGameObjectsInto(GameObject *parent)
{
    // We first store all the copies of the copies into a local list,
    // to avoid modifying the original copy which can be pasted multiple times.
    std::list<GameObject*> localCopies;
    for (GameObject *copy : s_copiedGameObjects)
    {
        GameObject *localCopy = static_cast<GameObject*>(copy->Clone());
        localCopies.push_back(localCopy);
    }

    for (GameObject *localCopy : localCopies)
    {
        GameObject *paste = static_cast<GameObject*>(localCopy);
        paste->SetParent(parent);
    }
}

void CopyPasteGameObjectManager::DuplicateCopiedGameObjects()
{
    std::list<GameObject*> localCopies;
    for (GameObject *copy : s_copiedGameObjects)
    {
        GameObject *localCopy = static_cast<GameObject*>(copy->Clone());
        localCopy->SetParent(s_goCopyToItsParent[copy]);
        localCopies.push_back(localCopy);
    }
}

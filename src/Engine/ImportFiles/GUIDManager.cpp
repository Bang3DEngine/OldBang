#include "Bang/GUIDManager.h"

#include "Bang/ImportFilesManager.h"

GUIDManager::GUIDManager()
{
}

GUID GUIDManager::GetNewGUID()
{
    GUID guid;

    do { guid = GUID::GetRandomGUID(); }
    while ( GUIDManager::IsBeingUsed(guid) );

    GUIDManager::GetInstance()->m_guids.Add(guid);
    return guid;
}

void GUIDManager::RemoveGUID(const GUID &guid)
{
    GUIDManager::GetInstance()->m_guids.Remove(guid);
}

bool GUIDManager::IsBeingUsed(const GUID &guid)
{
    return GUIDManager::GetInstance()->m_guids.Contains(guid);
}

GUIDManager *GUIDManager::GetInstance()
{
    return ImportFilesManager::GetGUIDManager();
}

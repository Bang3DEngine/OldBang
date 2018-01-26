#include "Bang/GUIDManager.h"

#include "Bang/Resources.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

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

void GUIDManager::CreateInsideFileGUID(const GUID &baseGUID,
                                       GUID::GUIDType insideFileGUID,
                                       GUID *outGUID)
{
    ASSERT_MSG(insideFileGUID > 0, "Inner file GUID's must be greater than zero!");

    *outGUID = baseGUID;
    outGUID->SetInsideFileGUID(insideFileGUID);
}

GUIDManager *GUIDManager::GetInstance()
{
    return ImportFilesManager::GetGUIDManager();
}

#ifndef GUIDMANAGER_H
#define GUIDMANAGER_H

#include "Bang/Set.h"
#include "Bang/GUID.h"

NAMESPACE_BANG_BEGIN

class GUIDManager
{
public:
    static GUID GetNewGUID();
    static void RemoveGUID(const GUID& guid);
    static bool IsBeingUsed(const GUID& guid);

    static void CreateInsideFileGUID(const GUID &baseGUID,
                                  GUID::GUIDType insideFileGUID,
                                  GUID *outGUID);

private:
    GUIDManager();

    Set<GUID> m_guids;

    static GUIDManager* GetInstance();

    friend class ImportFilesManager;
};

NAMESPACE_BANG_END

#endif // IMPORTFILESMANAGER_H

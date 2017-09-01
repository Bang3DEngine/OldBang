#ifndef GUIDMANAGER_H
#define GUIDMANAGER_H

#include "Bang/Set.h"
#include "Bang/GUID.h"

class GUIDManager
{
public:
    static GUID GetNewGUID();
    static void RemoveGUID(const GUID& guid);
    static bool IsBeingUsed(const GUID& guid);

private:
    GUIDManager();

    Set<GUID> m_guids;

    static GUIDManager* GetInstance();

    friend class ImportFilesManager;
};


#endif // IMPORTFILESMANAGER_H

#include "Bang/IGUIDable.h"

#include "Bang/GUIDManager.h"

USING_NAMESPACE_BANG

IGUIDable::IGUIDable()
{
}

IGUIDable::~IGUIDable()
{
}

void IGUIDable::SetGUID(const GUID &guid)
{
    m_GUID = guid;
    GUIDManager::RemoveGUID( GetGUID() );
}

const GUID& IGUIDable::GetGUID() const
{
    return m_GUID;
}

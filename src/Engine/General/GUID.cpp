#include "Bang/GUID.h"

USING_NAMESPACE_BANG

const GUID &GUID::Empty() { static GUID emptyGUID; return emptyGUID; }

bool GUID::IsEmpty() const { return *this == Empty(); }

GUID GUID::GetRandomGUID()
{
    GUID guid;
    guid.m_timeGUID = Time::GetNow_Nanos();
    guid.m_randGUID = Random::GetRange<GUIDType>(1, Math::Max<GUIDType>());

    return guid;
}

String GUID::ToString() const
{
    return String::ToString(m_timeGUID) + " " + String::ToString(m_randGUID);
}
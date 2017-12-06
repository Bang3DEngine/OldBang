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
    return String::ToString(m_timeGUID) + "_" + String::ToString(m_randGUID);
}

const GUID::GUIDType &GUID::GetTime() const { return m_timeGUID; }
const GUID::GUIDType &GUID::GetRand() const { return m_randGUID; }

#include "Bang/GUID.h"

USING_NAMESPACE_BANG

const GUID &GUID::Empty() { static GUID emptyGUID; return emptyGUID; }

bool GUID::IsEmpty() const { return *this == Empty(); }

GUID GUID::GetRandomGUID()
{
    GUID guid;
    GUIDType rand = Random::Range<GUIDType>(1, Math::Max<GUIDType>());

    constexpr uint TotalBits   = sizeof(GUIDType) * 8;
    constexpr uint TimeBits    = 60;
    static_assert(TimeBits <= TotalBits, "Too many TimeBits");
    constexpr uint RandomBits  = (TotalBits - TimeBits);
    constexpr uint NTimeBits   = (TotalBits-TimeBits);
    constexpr uint NRandomBits = (TotalBits-RandomBits);
    guid.m_guid = ((Time::GetNow() >> NTimeBits) << NTimeBits) +
                  ((rand << NRandomBits ) >> NRandomBits);
    return guid;
}

String GUID::ToString() const
{
    return String::ToString(m_guid);
}

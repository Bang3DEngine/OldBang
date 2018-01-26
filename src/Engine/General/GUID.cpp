#include "Bang/GUID.h"

USING_NAMESPACE_BANG

const GUID &GUID::Empty() { static GUID emptyGUID; return emptyGUID; }

bool GUID::IsEmpty() const { return *this == Empty(); }

void GUID::SetInsideFileGUID(const GUIDType &guid)
{
    m_insideFileGUID = guid;
}

GUID GUID::GetRandomGUID()
{
    GUID guid;
    guid.m_timeGUID = Time::GetNow_Nanos();
    guid.m_randGUID = Random::GetRange<GUIDType>(1, Math::Max<GUIDType>());
    guid.m_insideFileGUID = 0;
    return guid;
}

String GUID::ToString() const
{
    return String::ToString( GetTimeGUID() ) + " " +
           String::ToString( GetRandGUID() ) + " " +
           String::ToString( GetInsideFileGUID() );
}

const GUID::GUIDType &GUID::GetTimeGUID() const { return m_timeGUID; }
const GUID::GUIDType &GUID::GetRandGUID() const { return m_randGUID; }
const GUID::GUIDType &GUID::GetInsideFileGUID() const
{
    return m_insideFileGUID;
}

GUID GUID::WithoutInsideFileGUID() const
{
    GUID guid = *this;
    guid.SetInsideFileGUID(0);
    return guid;
}

std::istream &GUID::operator>>(std::istream &is)
{
    if (is.peek() != EOF && std::isdigit(is.peek())) { is >> m_timeGUID; }
    is >> std::ws;
    if (is.peek() != EOF && std::isdigit(is.peek())) { is >> m_randGUID; }
    is >> std::ws;
    if (is.peek() != EOF && std::isdigit(is.peek())) { is >> m_insideFileGUID; }
    return is;
}

bool GUID::operator==(const GUID &rhs) const
{
    return        GetTimeGUID() ==       rhs.GetTimeGUID() &&
                  GetRandGUID() ==       rhs.GetRandGUID() &&
            GetInsideFileGUID() == rhs.GetInsideFileGUID();
}

bool GUID::operator!=(const GUID &rhs) const
{
    return !(*this == rhs);
}

bool GUID::operator<(const GUID &rhs) const
{
    if (GetTimeGUID() < rhs.GetTimeGUID()) { return true; }
    else if (GetTimeGUID() > rhs.GetTimeGUID()) { return false; }
    else
    {
        if (GetRandGUID() < rhs.GetRandGUID()) { return true; }
        else if (GetRandGUID() > rhs.GetRandGUID()) { return false; }
        else
        {
            return (GetInsideFileGUID() < rhs.GetInsideFileGUID());
        }
    }
}

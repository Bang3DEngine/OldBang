#ifndef GUID_H
#define GUID_H

#include <istream>

#include "Bang/String.h"
#include "Bang/Random.h"
#include "Bang/IToString.h"

class GUID : public IToString
{
private:
    using GUIDType = long long unsigned int;
    static constexpr GUIDType EmptyGUID = 0;

public:
    GUID() {}

    static const GUID& Empty() { static GUID emptyGUID; return emptyGUID; }
    bool IsEmpty() const { return *this == Empty(); }

    static GUID GetRandomGUID()
    {
        GUID guid;
        guid.m_guid = Random::Range<GUIDType>(1, Math::Max<GUIDType>());
        return guid;
    }

    // IToString
    String ToString() const override
    {
        return String::ToString(m_guid);
    }

    friend  std::istream &operator>>(std::istream &is, GUID &guid);
    friend bool operator==(const GUID &lhs, const GUID &rhs);
    friend bool operator!=(const GUID &lhs, const GUID &rhs);
    friend bool operator<(const GUID &lhs, const GUID &rhs);

private:
    GUIDType m_guid = GUID::EmptyGUID;
};

inline std::istream &operator>>(std::istream &is, GUID &guid)
{
    is >> guid.m_guid;
    return is;
}

inline bool operator==(const GUID &lhs, const GUID &rhs)
{
    return lhs.m_guid == rhs.m_guid;
}
inline bool operator!=(const GUID &lhs, const GUID &rhs)
{
    return !(lhs == rhs);
}
inline bool operator<(const GUID &lhs, const GUID &rhs)
{
    return lhs.m_guid < rhs.m_guid;
}

#endif // GUID_H

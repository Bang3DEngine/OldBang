#ifndef GUID_H
#define GUID_H

#include <istream>

#include "Bang/Time.h"
#include "Bang/String.h"
#include "Bang/Random.h"
#include "Bang/IToString.h"

NAMESPACE_BANG_BEGIN

class GUID : public IToString
{
private:
    using GUIDType = uint64_t;
    static constexpr GUIDType EmptyGUID = 0;

public:
    GUID() {}

    static const GUID& Empty();
    bool IsEmpty() const;

    static GUID GetRandomGUID();

    // IToString
    String ToString() const override;

    friend  std::istream &operator>>(std::istream &is, GUID &guid);
    friend bool operator==(const GUID &lhs, const GUID &rhs);
    friend bool operator!=(const GUID &lhs, const GUID &rhs);
    friend bool operator<(const GUID &lhs, const GUID &rhs);

    const GUIDType& GetTime() const;
    const GUIDType& GetRand() const;

private:
    GUIDType m_timeGUID = GUID::EmptyGUID;
    GUIDType m_randGUID = GUID::EmptyGUID;
};

inline std::istream &operator>>(std::istream &is, GUID &guid)
{
    is >> guid.m_timeGUID;
    { char c; is >> c; }
    is >> guid.m_randGUID;
    return is;
}

inline bool operator==(const GUID &lhs, const GUID &rhs)
{
    return lhs.m_timeGUID == rhs.m_timeGUID &&
           lhs.m_randGUID == rhs.m_randGUID;
}
inline bool operator!=(const GUID &lhs, const GUID &rhs)
{
    return !(lhs == rhs);
}
inline bool operator<(const GUID &lhs, const GUID &rhs)
{
    if (lhs.m_timeGUID < rhs.m_timeGUID) { return true; }
    else if (lhs.m_timeGUID > rhs.m_timeGUID) { return false; }
    else
    {
        if (lhs.m_randGUID < rhs.m_randGUID) { return true; }
        return false;
    }
}

NAMESPACE_BANG_END

#endif // GUID_H

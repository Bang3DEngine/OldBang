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

NAMESPACE_BANG_END

#endif // GUID_H

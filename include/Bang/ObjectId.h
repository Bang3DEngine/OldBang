#ifndef OBJECTID_H
#define OBJECTID_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

// ObjectId
class ObjectId final
{
private:
    using ObjectIdType = uint64_t;

public:
    static ObjectId Empty;

    ~ObjectId() = default;
    ObjectIdType GetId() const;

private:
    static ObjectIdType NextObjectId;
    ObjectIdType m_id = Cast<ObjectIdType>(-1);

    ObjectId();

    friend bool operator==(const ObjectId &lhs, const ObjectId &rhs);
    friend bool operator==(const ObjectId &lhs, ObjectIdType id);
    friend bool operator==(ObjectIdType id, const ObjectId &rhs);
    friend bool operator!=(const ObjectId &lhs, const ObjectId &rhs);
    friend bool operator<(const ObjectId &lhs, const ObjectId &rhs);
    friend std::ostream& operator<<(std::ostream &os, const ObjectId &objectId);

    friend class Object;
    friend class ObjectManager;
};


inline bool operator==(const ObjectId &lhs, const ObjectId &rhs)
{ return lhs.m_id == rhs.m_id; }

inline bool operator==(typename ObjectId::ObjectIdType id, const ObjectId &rhs)
{ return id == rhs.m_id; }

inline bool operator==(const ObjectId &lhs, typename ObjectId::ObjectIdType id)
{ return lhs.m_id == id; }

bool operator!=(const ObjectId &lhs, const ObjectId &rhs);

inline bool operator<(const ObjectId &lhs, const ObjectId &rhs)
{ return lhs.m_id < rhs.m_id; }

inline std::ostream& operator<<(std::ostream &os, const ObjectId &objectId)
{ os << objectId.m_id; return os; }




NAMESPACE_BANG_END

#endif // OBJECTID_H


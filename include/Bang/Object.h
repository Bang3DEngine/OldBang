#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEnabledListener.h"
#include "Bang/IDestroyListener.h"

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

    static ObjectIdType s_nextObjectId;
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


// Object
class Object : public virtual Serializable,
               public IEnabledListener,
               public EventEmitter<IDestroyListener>,
               public EventEmitter<IEnabledListener>
{
public:
    const ObjectId& GetObjectId() const;
    void SetEnabled(bool enabled);

    bool IsEnabled() const;
    bool IsStarted() const;
    bool IsWaitingToBeDestroyed() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

protected:
    Object() = default;
    virtual ~Object();

    virtual void Start();
    virtual void OnStart();
    virtual void OnEnabled() override;
    virtual void OnDisabled() override;
    virtual void OnDestroy();

private:
    ObjectId m_objectId;
    bool m_enabled = true;
    bool m_started = false;
    bool m_waitingToBeDestroyed = false;

    void BeforeDestroyed();

    friend class ObjectManager;
};


// ObjectPointer
template <class ObjClass>
class ObjectPointer
{
private:
    ObjClass *p_object = nullptr;

public:
    ObjectPointer() = default;

    ObjectId GetObjectId() const;
    ObjClass* Get() const;
    void Set(ObjClass* object);

    bool operator==(const ObjectPointer &rhs) const;
    bool operator==(const ObjClass *rhs) const;
    bool operator==(const Object *rhs) const;
    bool operator!=(const ObjectPointer &rhs) const;
    bool operator!=(const ObjClass *rhs) const;
    bool operator!=(const Object *rhs) const;
    bool operator<(const ObjectPointer &rhs) const;
};
template<class ObjClass>
ObjectId ObjectPointer<ObjClass>::GetObjectId() const
{ return p_object ? p_object->GetObjectId() : ObjectId::Empty; }

template<class ObjClass>
ObjClass *ObjectPointer<ObjClass>::Get() const { return p_object; }

template<class ObjClass>
void ObjectPointer<ObjClass>::Set(ObjClass *object) { p_object = object; }

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator==(const ObjectPointer &rhs) const
{
    return *this == rhs.Get();
}

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator==(const ObjClass *rhs) const
{
    return *this == SCAST<const Object*>(rhs);
}

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator==(const Object *rhs) const
{
    if (Get() && rhs)
    {
        bool eq = GetObjectId() == rhs->GetObjectId();
        if (eq) { ASSERT(Get() == rhs); }
        return eq;
    }
    return !Get() && !rhs;
}

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator!=(const ObjectPointer &rhs) const
{ return !(*this == rhs); }

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator!=(const ObjClass *rhs) const
{ return !(*this == rhs); }

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator!=(const Object *rhs) const
{ return !(*this == rhs); }

template<class ObjClass>
bool ObjectPointer<ObjClass>::operator<(const ObjectPointer &rhs) const
{
    return GetObjectId() < rhs.GetObjectId();
}


NAMESPACE_BANG_END

#endif // OBJECT_H

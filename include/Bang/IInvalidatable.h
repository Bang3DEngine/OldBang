#ifndef IINVALIDATABLE_H
#define IINVALIDATABLE_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

#define IINVALIDATABLE(CLASS) \
        private: mutable bool m_invalid = true; \
        friend class IInvalidatable<CLASS>;

template <class T>
class IInvalidatable
{
public:
    virtual void Invalidate()
    {
        if (!IsInvalid())
        {
            SetInvalid(true);
            OnInvalidated();
        }
    }

    bool IsInvalid() const
    {
        return Cast<const T*>(this)->T::m_invalid;
    }

protected:
    IInvalidatable() {}
    virtual ~IInvalidatable() {}

    void Validate() const
    {
        SetInvalid(false);
    }

    virtual void OnInvalidated() {}

private:
    void SetInvalid(bool invalid) const
    {
        Cast<const T*>(this)->T::m_invalid = invalid;
    }

};

NAMESPACE_BANG_END

#endif // IINVALIDATABLE_H_H


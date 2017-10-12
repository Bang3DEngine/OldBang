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
    void Invalidate()
    {
        if (!IsInvalid())
        {
            SetInvalid(true);
            OnInvalidated();
        }
    }

protected:
    IInvalidatable() {}
    virtual ~IInvalidatable() {}

    void Validate() const
    {
        SetInvalid(false);
    }

    bool IsInvalid() const
    {
        return SCAST<const T*>(this)->m_invalid;
    }

private:
    void SetInvalid(bool invalid) const
    {
        SCAST<const T*>(this)->m_invalid = invalid;
    }

    virtual void OnInvalidated() {}
};

NAMESPACE_BANG_END

#endif // IINVALIDATABLE_H_H


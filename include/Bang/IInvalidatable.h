#ifndef IINVALIDATABLE_H
#define IINVALIDATABLE_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

template <class T>
class IInvalidatable
{
public:
    virtual void Invalidate()
    {
        if (!IsInvalid() || IsFirstInvalidation())
        {
            m_isFirstInvalidation = false;
            SetInvalid(true);
            OnInvalidated();
        }
    }

    bool IsInvalid() const { return m_invalid; }

protected:
    IInvalidatable() {}
    virtual ~IInvalidatable() {}

    void Validate() const
    {
        SetInvalid(false);
    }

    bool IsFirstInvalidation() const { return m_isFirstInvalidation; }

    virtual void OnInvalidated() {}

private:
    mutable bool m_invalid = true;
    mutable bool m_isFirstInvalidation = true;

    void SetInvalid(bool invalid) const
    {
        m_invalid = invalid;
    }
};

NAMESPACE_BANG_END

#endif // IINVALIDATABLE_H_H


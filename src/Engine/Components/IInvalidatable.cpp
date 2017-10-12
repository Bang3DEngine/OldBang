#include "Bang/IInvalidatable.h"

USING_NAMESPACE_BANG

void IInvalidatable::Invalidate()
{
    if (!IsInvalid())
    {
        SetInvalid(true);
        OnInvalidated();
    }
}

void IInvalidatable::Validate() const
{
    SetInvalid(false);
}

bool IInvalidatable::IsInvalid() const
{
    return m_invalid;
}

void IInvalidatable::SetInvalid(bool invalid) const
{
    m_invalid = invalid;
}

IInvalidatable::IInvalidatable()  {}
IInvalidatable::~IInvalidatable() {}

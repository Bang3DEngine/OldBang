#ifndef IINVALIDATABLE_H
#define IINVALIDATABLE_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IInvalidatable
{
public:
    void Invalidate();

protected:
	IInvalidatable();
	virtual ~IInvalidatable();

    void Validate() const;
    bool IsInvalid() const;

private:
    mutable bool m_invalid = false;

    void SetInvalid(bool invalid) const;
    virtual void OnInvalidated() {}
};

NAMESPACE_BANG_END

#endif // IINVALIDATABLE_H_H


#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class ILayoutController
{
public:
    void ApplyLayout();
    void Invalidate();

protected:
    ILayoutController();
    virtual ~ILayoutController();

private:
    bool m_isInvalid = true;

    virtual void _ApplyLayout() = 0;
    bool IsInvalid() const;
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H

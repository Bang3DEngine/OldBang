#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include "Bang/IInvalidatable.h"

NAMESPACE_BANG_BEGIN

class ILayoutController : public IInvalidatable<ILayoutController>
{
    IINVALIDATABLE(ILayoutController)

public:
    void ApplyLayout();

protected:
    ILayoutController();
    virtual ~ILayoutController();

private:
    virtual void _ApplyLayout() = 0;
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H

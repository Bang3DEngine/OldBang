#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class ILayoutController
{
public:
    virtual void ApplyLayout() = 0;

protected:
    ILayoutController();
    virtual ~ILayoutController();
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H

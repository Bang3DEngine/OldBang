#include "Bang/ILayoutController.h"

#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

ILayoutController::ILayoutController() {}
ILayoutController::~ILayoutController() {}

void ILayoutController::Invalidate()
{
    IInvalidatable<ILayoutController>::Invalidate();
}

void ILayoutController::OnInvalidated()
{
    IInvalidatable<ILayoutController>::OnInvalidated();
    UILayoutManager::PropagateInvalidation(this);
}

void ILayoutController::_ApplyLayout(Axis axis)
{
    if (IInvalidatable<ILayoutController>::IsInvalid())
    {
        ApplyLayout(axis);
        if (axis == Axis::Vertical) { Validate(); }
    }
}

void ILayoutController::OnChildAdded(GameObject *addedChild,
                                     GameObject *parent)
{
    (void) addedChild; (void) parent;
    Invalidate();
}

void ILayoutController::OnChildRemoved(GameObject *removedChild,
                                       GameObject *parent)
{
    (void) removedChild; (void) parent;
    Invalidate();
}

void ILayoutController::OnTransformChanged()
{
    Invalidate();
}

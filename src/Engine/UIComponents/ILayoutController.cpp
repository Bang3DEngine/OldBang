#include "Bang/ILayoutController.h"

#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

ILayoutController::ILayoutController() {}
ILayoutController::~ILayoutController() {}

void ILayoutController::OnInvalidated()
{
    IInvalidatable<ILayoutController>::OnInvalidated();
    UILayoutManager::OnInvalidated(this);
}

void ILayoutController::_ApplyLayout(Axis axis)
{
    if (IInvalidatable<ILayoutController>::IsInvalid())
    {
        ApplyLayout(axis);
        if (axis == Axis::Vertical) { Validate(); }
    }
}

void ILayoutController::OnChildAdded(GameObject *addedChild)
{
    Invalidate();
}

void ILayoutController::OnChildRemoved(GameObject *removedChild)
{
    Invalidate();
}

void ILayoutController::OnParentChanged(GameObject *oldParent,
                                        GameObject *newParent)
{
    Invalidate();
}

void ILayoutController::OnTransformChanged()
{
    Invalidate();
}

void ILayoutController::OnChildrenTransformChanged()
{
    Invalidate();
}

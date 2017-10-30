#include "Bang/ILayoutController.h"

#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

void ILayoutController::OnInvalidated()
{
    IInvalidatable<ILayoutController>::OnInvalidated();
    UILayoutManager::OnInvalidated(this);
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

void ILayoutController::OnRectTransformChanged()
{
    Invalidate();
}

void ILayoutController::OnChildrenRectTransformChanged()
{
    Invalidate();
}

ILayoutController::ILayoutController() {}
ILayoutController::~ILayoutController() {}

#include "Bang/ILayoutController.h"

#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

#include "Bang/GL.h"
#include "Bang/Random.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
void ILayoutController::ApplyLayout()
{
    // if (IsInvalid())
    {
        // Debug_Log("Applying layout..." << DCAST<Component*>(this)->
        //                                   GetGameObject()->GetName());
        _ApplyLayout();
        Validate();
    }
}

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

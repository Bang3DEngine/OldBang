#include "Bang/ILayoutController.h"

USING_NAMESPACE_BANG

#include "Bang/Random.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
void ILayoutController::ApplyLayout()
{
    if (IsInvalid())
    {
        Debug_Log("Applying layout..." << DCAST<Component*>(this)->
                                          GetGameObject()->GetName());

        _ApplyLayout();
        Validate();
    }
}

ILayoutController::ILayoutController() {}
ILayoutController::~ILayoutController() {}

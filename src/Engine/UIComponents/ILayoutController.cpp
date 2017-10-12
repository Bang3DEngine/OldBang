#include "Bang/ILayoutController.h"

USING_NAMESPACE_BANG

#include "Bang/Random.h"
void ILayoutController::ApplyLayout()
{
    if (IsInvalid())
    {
        Debug_Log("Applying layout..." << Random::Value());

        _ApplyLayout();
        Validate();
    }
}

ILayoutController::ILayoutController() {}
ILayoutController::~ILayoutController() {}

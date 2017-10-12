#include "Bang/ILayoutController.h"

void Bang::ILayoutController::ApplyLayout()
{
    if (IsInvalid())
    {
        _ApplyLayout();
        m_isInvalid = false;
    }
}

void Bang::ILayoutController::Invalidate()
{
    m_isInvalid = true;
}

Bang::ILayoutController::ILayoutController()
{

}

Bang::ILayoutController::~ILayoutController()
{

}

bool Bang::ILayoutController::IsInvalid() const
{
    return m_isInvalid;
}

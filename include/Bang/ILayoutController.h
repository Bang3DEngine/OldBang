#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include "Bang/IInvalidatable.h"
#include "Bang/IChildrenListener.h"
#include "Bang/IRectTransformListener.h"

NAMESPACE_BANG_BEGIN

class ILayoutController : public IInvalidatable<ILayoutController>,
                          public IChildrenListener,
                          public IRectTransformListener
{
    IINVALIDATABLE(ILayoutController)

public:
    void ApplyLayout();
    virtual void OnInvalidated() override;

    // IChildrenListener
    void OnChildAdded(GameObject *addedChild) override;
    void OnChildRemoved(GameObject *removedChild) override;
    void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

    // IRectTransformListener
    void OnRectTransformChanged() override;
    void OnChildrenRectTransformChanged() override;

protected:
    ILayoutController();
    virtual ~ILayoutController();

private:
    virtual void _ApplyLayout() = 0;
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H

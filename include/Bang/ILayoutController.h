#ifndef ILAYOUTCONTROLLER_H
#define ILAYOUTCONTROLLER_H

#include "Bang/Axis.h"
#include "Bang/IInvalidatable.h"
#include "Bang/IChildrenListener.h"
#include "Bang/ITransformListener.h"

NAMESPACE_BANG_BEGIN

class ILayoutController : public IInvalidatable<ILayoutController>,
                          public IChildrenListener,
                          public ITransformListener
{
public:
    // IInvalidatable
    virtual void Invalidate() override;
    virtual void OnInvalidated() override;

    // IChildrenListener
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;
    void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

    // ITransformListener
    void OnTransformChanged() override;
    void OnParentTransformChanged() override;

protected:
    ILayoutController();
    virtual ~ILayoutController();

private:
    virtual void ApplyLayout(Axis axis) = 0;
    void _ApplyLayout(Axis axis);

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // ILAYOUTCONTROLLER_H

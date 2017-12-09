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
    IINVALIDATABLE(ILayoutController)

public:
    virtual void OnInvalidated() override;

    // IChildrenListener
    virtual void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;

    // ITransformListener
    void OnTransformChanged() override;
    void OnChildrenTransformChanged() override;

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

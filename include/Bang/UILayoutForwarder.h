#ifndef UILAYOUTFORWARDER_H
#define UILAYOUTFORWARDER_H

#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"

NAMESPACE_BANG_BEGIN

class UILayoutForwarder : public Component,
                          public ILayoutElement
{
    COMPONENT(UILayoutForwarder)

public:
	UILayoutForwarder();
	virtual ~UILayoutForwarder();

    void CalculateLayout(Axis axis) override;
};

NAMESPACE_BANG_END

#endif // UILAYOUTFORWARDER_H


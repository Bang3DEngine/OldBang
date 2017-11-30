#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <functional>

#include "Bang/Set.h"
#include "Bang/Input.h"
#include "Bang/Component.h"
#include "Bang/IFocusable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;

class UIButtoneable : public Component,
                      public IFocusable
{
    COMPONENT(UIButtoneable)

public:
    UIButtoneable();
    virtual ~UIButtoneable();

    // Component
    virtual void OnUpdate() override;

    using ClickedCallback = std::function<void(UIButtoneable*)>;
    void AddClickedCallback(ClickedCallback callback);

    bool IsBeingPressed() const;

private:
    bool m_beingPressed = false;
    Array<ClickedCallback> m_clickedCallbacks;
};


NAMESPACE_BANG_END

#endif // UIBUTTON_H

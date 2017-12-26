#ifndef IFOCUSABLE_H
#define IFOCUSABLE_H

#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

class IFocusable : public EventEmitter<IFocusListener>
{
public:
    bool IsMouseOver() const;

    void SetFocusEnabled(bool focusEnabled);

    void Click(bool doubleClick);
    bool HasFocus() const;
    bool IsFocusEnabled() const;
    bool HasJustFocusChanged() const;

    bool IsBeingPressed() const;

    using ClickedCallback = std::function<void(IFocusable*)>;
    void AddClickedCallback(ClickedCallback callback);
    void AddDoubleClickedCallback(ClickedCallback callback);


protected:
    IFocusable();
    virtual ~IFocusable();

    void SetFocus();
    void ClearFocus();
    virtual void PropagateFocusToListeners();
    virtual void PropagateOnClickedToListeners(bool doubleClick);
    virtual void PropagateMouseOverToListeners(bool mouseOver);

private:
    bool m_beingPressed = false;
    bool m_hasFocus = false;
    bool m_isMouseOver = false;
    bool m_focusEnabled = true;
    bool m_hasJustFocusChanged = false;

    Array<ClickedCallback> m_clickedCallbacks;
    Array<ClickedCallback> m_doubleClickedCallbacks;

    void UpdateFromCanvas();

    friend class UICanvas;
};

NAMESPACE_BANG_END

#endif // IFOCUSABLE_H


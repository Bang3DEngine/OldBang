#ifndef IFOCUSABLE_H
#define IFOCUSABLE_H

#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

class IFocusable : public EventEmitter<IFocusListener>
{
public:
    bool HasMouseOver() const;

    void SetFocusEnabled(bool focusEnabled);

    bool HasFocus() const;
    bool IsFocusEnabled() const;

protected:
    IFocusable();
    virtual ~IFocusable();

    virtual void SetFocus();
    virtual void ClearFocus();
    virtual void PropagateToFocusListeners();

private:
    bool m_hasFocus = false;
    bool m_focusEnabled = true;
    bool m_hasMouseOver = false;

    void HandleFocusing(bool handleFocusTake);

    friend class UICanvas;
};

NAMESPACE_BANG_END

#endif // IFOCUSABLE_H


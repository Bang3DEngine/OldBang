#ifndef IFOCUSLISTENER_H
#define IFOCUSLISTENER_H

enum class FocusAction { TakeIt, PassToParent };

class IFocusListener
{
public:
    virtual void OnFocusTaken() {}
    virtual void OnFocusLost()  {}
};

#endif // IFOCUSLISTENER_H

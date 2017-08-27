#ifndef IFOCUSABLE_H
#define IFOCUSABLE_H

enum class FocusAction { TakeIt, PassToParent };

class IFocusable
{
public:
    virtual void OnFocusTaken() {}
    virtual void OnFocusLost()  {}
};

#endif // IFOCUSABLE_H

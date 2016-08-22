#ifndef ISHORTCUTLISTENER_H
#define ISHORTCUTLISTENER_H

#include "Input.h"

class ShortcutManager;
class IShortcutListener
{
public:
    IShortcutListener();
    virtual ~IShortcutListener();

    virtual void OnShortcutPressed() = 0;
    virtual void OnShortcutPressedKey(Input::Key key);
};

#endif // ISHORTCUTLISTENER_H

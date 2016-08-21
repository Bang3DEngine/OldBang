#ifndef ISHORTCUTLISTENER_H
#define ISHORTCUTLISTENER_H

class IShortcutListener
{
public:
    IShortcutListener();
    virtual ~IShortcutListener();

    virtual void OnShortcutsUpdate() = 0;
};

#endif // ISHORTCUTLISTENER_H

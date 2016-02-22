#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H


class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();

public:
    virtual void OnMenuBarItemClicked(int itemClicked) { };
};

#endif // IWINDOWEVENTMANAGERLISTENER_H

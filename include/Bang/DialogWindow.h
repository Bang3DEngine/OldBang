#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include "Bang/Window.h"

NAMESPACE_BANG_BEGIN

class DialogWindow : public Window
{
public:
    DialogWindow(Window *parentWindow, bool resizable);
    virtual ~DialogWindow();

    virtual void Create(uint flags) override;
    void CenterInParent();

private:
    bool m_resizable = false;
    Window *p_parentWindow = nullptr;
};

NAMESPACE_BANG_END

#endif // DIALOGWINDOW_H

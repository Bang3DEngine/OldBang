#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class DialogWindow;
FORWARD class UIInteractive;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);

    Dialog() = delete;

private:
    static Scene* CreateScene(const String &msg);

    static void OnButtonClicked(UIInteractive *interactive);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

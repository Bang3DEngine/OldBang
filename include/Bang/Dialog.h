#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class DialogWindow;
FORWARD class UIInteractive;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);

    static Path GetFile(const String &title);

    Dialog() = delete;

private:
    static Scene* CreateOpenFileScene(const String &title);
    static Scene* CreateMsgScene(const String &msg);

    static void OnButtonClicked(UIInteractive *interactive);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

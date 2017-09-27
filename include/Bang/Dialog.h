#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class DialogWindow;
FORWARD class UIButton;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);

    static Path GetFilePath(const String &title);

    Dialog() = delete;

private:
    static Path s_resultPath;

    static Scene* CreateGetFilePathScene(const String &title);
    static Scene* CreateMsgScene(const String &msg);

    static void OnButtonClicked(UIButton *button);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

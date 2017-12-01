#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class IFocusable;
FORWARD class DialogWindow;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);

    static Path GetFilePath(const String &title,
                            const List<String> &extensions = {});

    Dialog() = delete;

private:
    static Path s_resultPath;

    static Scene* CreateGetFilePathScene(const String &title,
                                         const List<String> &extensions);
    static Scene* CreateMsgScene(const String &msg);

    static void FileAcceptedCallback(const Path &path);

    static void OnButtonClicked(IFocusable *button);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

#include "Bang/Path.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class UIButton;
FORWARD class IFocusable;
FORWARD class UIFileList;
FORWARD class UIInputText;
FORWARD class DialogWindow;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);
    static Path OpenFilePath(const String &title,
                             const List<String> &extensions = {});
    static Path SaveFilePath(const String &title,
                             const String &extension);

    Dialog() = delete;

private:
    static Path s_resultPath;

    static DialogWindow* BeginCreateDialog(const String &title);
    static void EndCreateDialog(DialogWindow *dialogWindow);

    static void CreateSaveFilePathSceneInto(Scene *scene,
                                            const String &extension);
    static void CreateOpenFilePathSceneInto(Scene *scene,
                                            const List<String> &extensions);
    static void CreateFilePathBaseInto(Scene *scene,
                                       UIFileList **outFileList,
                                       UIButton **outBotLeftButton,
                                       UIButton **outBotRightButton,
                                       UIInputText **botInputText);

    static Scene* CreateMsgScene(const String &msg);

    static void AcceptDialogFile(const Path &path);

    static void OnButtonClicked(IFocusable *button);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

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
    static String GetString(const String &title,
                            const String &msg,
                            const String &hint = "");
    static Path OpenFilePath(const String &title,
                             const List<String> &extensions = {},
                             const Path &initialDirPath = Path::Empty);
    static Path OpenDirectory(const String &title,
                              const Path &initialDirPath = Path::Empty);
    static Path SaveFilePath(const String &title,
                             const String &extension,
                             const Path &initialDirPath = Path::Empty);

    Dialog() = delete;

private:
    static bool s_okPressed;
    static Path s_resultPath;
    static String s_resultString;
    static DialogWindow *s_currentDialog;

    static DialogWindow* BeginCreateDialog(const String &title);
    static void EndCreateDialog(DialogWindow *dialogWindow);
    static void EndCurrentDialog();

    static void CreateSaveFilePathSceneInto(Scene *scene,
                                            const String &extension,
                                            const Path &initialDirPath);
    static void CreateOpenFilePathSceneInto(Scene *scene,
                                            bool openDir,
                                            const List<String> &extensions,
                                            const Path &initialDirPath);
    static void CreateFilePathBaseInto(Scene *scene,
                                       const Path &initialDirPath,
                                       UIFileList **outFileList,
                                       UIButton **outBotLeftButton,
                                       UIButton **outBotRightButton,
                                       UIInputText **botInputText);
    static Scene* CreateGetStringScene(const String &msg,
                                       const String &hint);

    static Scene* CreateMsgScene(const String &msg);

    static void AcceptDialogPath(const Path &path);
    static void OnDialogPathChanged(const Path &path);
    static void OnAcceptButtonClicked(IFocusable *button);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

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
    enum YesNoCancel { Yes, No, Cancel };

    static DialogWindow* Error(const String &title,
                               const String &msg);
    static String GetString(const String &title,
                            const String &msg,
                            const String &hint = "");
    static YesNoCancel GetYesNoCancel(const String &title, const String &msg);
    static Path OpenFilePath(const String &title,
                             const List<String> &extensions = {},
                             const Path &initialDirPath = Path::Empty);
    static Path OpenDirectory(const String &title,
                              const Path &initialDirPath = Path::Empty);
    static Path SaveFilePath(const String &title,
                             const String &extension,
                             const Path &initialDirPath = Path::Empty,
                             const String &initialFileName = "Unnamed");

    static DialogWindow* BeginCreateDialog(const String &title,
                                           int sizeX, int sizeY,
                                           bool resizable);
    static void EndCreateDialog(DialogWindow *dialogWindow);
    static void EndCurrentDialog();

    Dialog() = delete;

private:
    static bool s_okPressed;
    static Path s_resultPath;
    static String s_resultString;
    static YesNoCancel s_resultYesNoCancel;

    static DialogWindow *s_currentDialog;


    static void CreateSaveFilePathSceneInto(Scene *scene,
                                            const String &extension,
                                            const Path &initialDirPath,
                                            const String &initialFileName);
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
    static Scene* CreateYesNoCancelScene(const String &msg);

    static Scene* CreateMsgScene(const String &msg);

    static void OnOkClicked(IFocusable *button);
    static void OnYesClicked(IFocusable *button);
    static void OnNoClicked(IFocusable *button);
    static void OnCancelClicked(IFocusable *button);
    static void AcceptDialogPath(const Path &path);
    static void OnDialogPathChanged(const Path &path);
    static void OnNeedToEndDialog(IFocusable *button);
};

NAMESPACE_BANG_END

#endif // DIALOG_H

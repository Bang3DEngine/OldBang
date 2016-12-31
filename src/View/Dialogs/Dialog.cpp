#include "Dialog.h"

#include <QInputDialog>

#include "EditorWindow.h"
#include "SelectProjectWindow.h"

Dialog::Dialog()
{
}

String Dialog::GetExtensionFilterString(const String &extension)
{
    String ext = extension;
    StringUtils::AddInFrontOfWords("*.", &ext);
    return ext + "(" + ext + ")" ;
}

QWidget *Dialog::GetCurrentWindow()
{
    if (EditorWindow::GetInstance())
    {
        QWidget *w = EditorWindow::GetInstance()->GetMainWindow();
        if (w) { return w; }
    }

    if (SelectProjectWindow::GetInstance())
    {
        QWidget *w = SelectProjectWindow::GetInstance()->GetMainWindow();
        if (w) { return w; }
    }

    return nullptr;
}

String Dialog::GetOpenDirname(const String &caption,
                                  const String &initDirectory)
{
    String dir =
            String(
                QFileDialog::getExistingDirectory(
                    Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    initDirectory.ToQString(),
                    QFileDialog::ShowDirsOnly |
                    QFileDialog::DontResolveSymlinks //|
                    //QFileDialog::DontUseNativeDialog
                )
            );

    return dir;
}

String Dialog::GetOpenFilename(const String &caption,
                                   const String &extension,
                                   const String &initDir)
{
    String filepath =
            String(
                QFileDialog::getOpenFileName(
                    Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    initDir.ToQString(),
                    Dialog::GetExtensionFilterString(extension).ToQString(),
                    nullptr,
                    0 // QFileDialog::DontUseNativeDialog
                )
            );

    return filepath;
}

String Dialog::GetSaveFilename(const String &caption,
                                   const String &extension,
                                   const String &initPath,
                                   const String &suggestedFilename)
{
    String filepath =
            String(
                QFileDialog::getSaveFileName(
                    Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    String(initPath + "/" + suggestedFilename).ToQString(),
                    Dialog::GetExtensionFilterString(extension).ToQString(),
                    nullptr,
                    0 // QFileDialog::DontUseNativeDialog
                )
            );

    //This function auto detects if it was in the filepath already.
    filepath = Persistence::AppendExtension(filepath, extension);
    return filepath;
}


String Dialog::GetInputString(const String &caption,
                              const String &labelText,
                              const String &defaultText,
                              bool *ok)
{
    bool everythingOk = false;
    String str =
            String(
                QInputDialog::getText(
                    Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    labelText.ToQString(),
                    QLineEdit::Normal,
                    defaultText.ToQString(),
                    &everythingOk)
            );

    if (ok) { *ok = everythingOk; }
    return str;
}

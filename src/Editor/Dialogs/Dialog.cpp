#include "Bang/Dialog.h"

#include <QInputDialog>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/EditorWindow.h"

Dialog::Dialog()
{
}

String Dialog::GetExtensionFilterString(const String &extension)
{
    String ext = extension.AddInFrontOfWords("*.");
    return ext + "(" + ext + ")" ;
}

QWidget *Dialog::GetCurrentWindow()
{
    if (EditorWindow::GetInstance())
    {
        QWidget *w = EditorWindow::GetInstance()->GetMainWindow();
        if (w) { return w; }
    }

    return nullptr;
}

Path Dialog::GetOpenDirpath(const String &caption, const Path &initDirectory)
{
    Path dir(QFileDialog::getExistingDirectory(
                    Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    initDirectory.GetAbsolute().ToQString(),
                    QFileDialog::ShowDirsOnly |
                    QFileDialog::DontResolveSymlinks //|
                    //QFileDialog::DontUseNativeDialog
              ));

    if (!dir.IsDir()) { return Path::Empty; }
    return dir;
}

Path Dialog::GetOpenFilepath(const String &caption,
                             const String &extension,
                             const Path &initDir)
{
    Path filepath(QFileDialog::getOpenFileName(
                      Dialog::GetCurrentWindow(),
                      caption.ToQString(),
                      initDir.GetAbsolute().ToQString(),
                      Dialog::GetExtensionFilterString(extension).ToQString(),
                      nullptr,
                      0 // QFileDialog::DontUseNativeDialog
                ));

    if (!filepath.IsFile()) { return Path::Empty; }
    return filepath;
}

Path Dialog::GetSaveFilepath(const String &caption,
                             const String &extension,
                             const Path &initPath,
                             const String &suggestedFilename,
                             QWidget *parent)
{
    Path filepath(QFileDialog::getSaveFileName(
                    parent ? parent : Dialog::GetCurrentWindow(),
                    caption.ToQString(),
                    initPath.Append(suggestedFilename).GetAbsolute().ToQString(),
                    Dialog::GetExtensionFilterString(extension).ToQString(),
                    nullptr,
                    0 // QFileDialog::DontUseNativeDialog
                ));
    if (!filepath.IsEmpty())
    {
        filepath.AppendExtension(extension);
    }
    return filepath;
}


String Dialog::GetInputString(const String &caption,
                              const String &labelText,
                              const String &defaultText,
                              bool *ok)
{
    bool everythingOk = false;
    String str(QInputDialog::getText(
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

Dialog::Reply Dialog::GetYesNo(const String &caption,
                               const String &labelText)
{

    QMessageBox::StandardButton reply =
            QMessageBox::question(Dialog::GetCurrentWindow(),
                                  caption.ToQString(),
                                  labelText.ToQString(),
                                  (QMessageBox::Yes |  QMessageBox::No)
                                  );
    return static_cast<Dialog::Reply>(reply);
}

Dialog::Reply Dialog::GetYesNoCancel(const String &caption,
                                     const String &labelText)
{

    QMessageBox::StandardButton reply =
            QMessageBox::question(Dialog::GetCurrentWindow(),
                                  caption.ToQString(),
                                  labelText.ToQString(),
                                  (QMessageBox::Yes |  QMessageBox::No |
                                   QMessageBox::Cancel)
                                  );
    return static_cast<Dialog::Reply>(reply);
}

void Dialog::Error(const String &caption,
                   const String &labelText)
{
    QMessageBox::critical(Dialog::GetCurrentWindow(),
                          caption.ToQString(),
                          labelText.ToQString());
}

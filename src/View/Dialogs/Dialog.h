#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "String.h"
#include "Persistence.h"

class Dialog
{
public:
    enum Reply
    {
        Yes    = QMessageBox::Yes,
        No     = QMessageBox::No,
        Cancel = QMessageBox::Cancel
    };

    static String GetOpenDirname(const String &caption = "Choose a directory",
                                 const String &initDirectory = String(QDir::homePath()) );
    static String GetOpenFilename(const String &caption = "Open file",
                                  const String &extension = "*",
                                  const String &initDir = Persistence::GetProjectAssetsRootAbs());
    static String GetSaveFilename(const String &caption = "Save file",
                                  const String &extension = "*",
                                  const String &initPath = Persistence::GetProjectAssetsRootAbs(),
                                  const String &suggestedFilename = "",
                                  QWidget *parent = nullptr);

    static String GetInputString(const String &caption = "Input string",
                                 const String &labelText = "Your string:",
                                 const String &defaultText = "DefaultString",
                                 bool *ok = nullptr);


    static Dialog::Reply GetYesNo(const String &caption = "Confirm",
                                  const String &labelText = "Are you sure?");
    static Dialog::Reply GetYesNoCancel(const String &caption = "Confirm",
                                        const String &labelText = "Are you sure?");

    static void Error(const String &caption = "Error",
                      const String &labelText = "There has been an error.");

private:
    Dialog();
    static String GetExtensionFilterString(const String &extension);
    static QWidget *GetCurrentWindow();
};

#endif // FILEDIALOG_H

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "String.h"
#include "Persistence.h"

class Dialog
{
private:

    Dialog();
    static String GetExtensionFilterString(const String &extension);
    static QWidget *GetCurrentWindow();

public:

    static String GetOpenDirname(const String &caption = "Choose a directory",
                                 const String &initDirectory = String(QDir::homePath()) );
    static String GetOpenFilename(const String &caption = "Open file",
                                  const String &extension = "*",
                                  const String &initDir = Persistence::GetAssetsPathAbsolute());
    static String GetSaveFilename(const String &caption = "Save file",
                                  const String &extension = "*",
                                  const String &initPath = Persistence::GetAssetsPathAbsolute(),
                                  const String &suggestedFilename = "");

    static String GetInputString(const String &caption = "Input string",
                                 const String &labelText = "Your string:",
                                 const String &defaultText = "DefaultString",
                                 bool *ok = nullptr);
};

#endif // FILEDIALOG_H

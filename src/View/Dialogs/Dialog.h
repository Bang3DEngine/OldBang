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
};

#endif // FILEDIALOG_H

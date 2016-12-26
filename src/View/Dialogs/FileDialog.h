#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

#include "String.h"

class FileDialog : public QFileDialog
{
private:
    String m_title = "";
    String m_extension = "";

    String GetExtensionFilterString(const String &extension);

public:
    FileDialog(const String &title,
               const String &extension);

    String GetOpenFilename();
    String GetSaveFilename(const String &suggestedFilename = "");
};

#endif // FILEDIALOG_H

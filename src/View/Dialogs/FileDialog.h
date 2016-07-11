#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <string>
#include <QFileDialog>


class FileDialog : public QFileDialog
{
private:
    std::string m_title = "";
    std::string m_extension = "";

    std::string GetExtensionFilterString(const std::string &extension);

public:
    FileDialog(const std::string &title,
               const std::string &extension);

    std::string GetOpenFilename();
    std::string GetSaveFilename(const std::string &suggestedFilename = "");
};

#endif // FILEDIALOG_H

#include "FileDialog.h"

#include "WindowMain.h"
#include "Persistence.h"

FileDialog::FileDialog(const std::string &title,
                       const std::string &extension) :
    QFileDialog(WindowMain::GetMainWindow())
{
    this->title = title;
    this->extension = extension;
}

std::string FileDialog::GetExtensionFilterString(const std::string &extension)
{
    return extension + "(*." + extension + ")" ;
}


std::string FileDialog::GetOpenFilename()
{
    std::string filepath =
            this->getOpenFileName(
                WindowMain::GetMainWindow(),
                QString::fromStdString(title),
                QString::fromStdString(Persistence::GetAssetsPathAbsolute()),
                QString::fromStdString(GetExtensionFilterString(extension))
            ).toStdString();

    return filepath;
}

std::string FileDialog::GetSaveFilename(const std::string &suggestedFilename)
{
    std::string dirpath = Persistence::GetAssetsPathAbsolute() +
            (suggestedFilename == "" ? "" : ("/" + suggestedFilename));

    std::string filepath =
            this->getSaveFileName(
                WindowMain::GetMainWindow(),
                QString::fromStdString(title),
                QString::fromStdString(dirpath),
                QString::fromStdString(GetExtensionFilterString(extension))
            ).toStdString();

    //This function auto detects if it was in the filepath already.
    filepath = Persistence::AppendExtension(filepath, extension);

    return filepath;
}

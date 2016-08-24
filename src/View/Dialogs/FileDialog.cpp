#include "FileDialog.h"

#include "EditorWindow.h"
#include "Persistence.h"

FileDialog::FileDialog(const String &title,
                       const String &extension) :
    QFileDialog(EditorWindow::GetInstance()->GetMainWindow())
{
    this->m_title = title;
    this->m_extension = extension;
}

String FileDialog::GetExtensionFilterString(const String &extension)
{
    String ext = extension;
    StringUtils::AddInFrontOfWords("*.", &ext);
    return ext + "(" + ext + ")" ;
}

String FileDialog::GetOpenFilename()
{
    String filepath =
            this->getOpenFileName(
                EditorWindow::GetInstance()->GetMainWindow(),
                QString::fromStdString(m_title),
                QString::fromStdString(Persistence::GetAssetsPathAbsolute()),
                QString::fromStdString(GetExtensionFilterString(m_extension)),
                nullptr,
                QFileDialog::DontUseNativeDialog
            ).toStdString();

    return filepath;
}

String FileDialog::GetSaveFilename(const String &suggestedFilename)
{
    String dirpath = Persistence::GetAssetsPathAbsolute() +
            (suggestedFilename == "" ? "" : ("/" + suggestedFilename));

    String filepath =
            getSaveFileName(
                EditorWindow::GetInstance()->GetMainWindow(),
                QString::fromStdString(m_title),
                QString::fromStdString(dirpath),
                QString::fromStdString(GetExtensionFilterString(m_extension)),
                nullptr,
                QFileDialog::DontUseNativeDialog
            ).toStdString();

    //This function auto detects if it was in the filepath already.
    filepath = Persistence::AppendExtension(filepath, m_extension);
    return filepath;
}

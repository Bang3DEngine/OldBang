#include "GameBuildDialog.h"

#include "EditorWindow.h"

GameBuildDialog::GameBuildDialog() :
    QProgressDialog("Building game...",
                    "Cancel",
                    0,
                    c_maxTotalProgress)
{
    setModal(true);
    setWindowTitle("Game Building");
    setWindowModality(Qt::WindowModality::ApplicationModal);

    show();
    raise();
    activateWindow();
}

void GameBuildDialog::SetPercent(float percent)
{
    setValue(percent * c_maxTotalProgress);
}

void GameBuildDialog::SetMessage(const String &message)
{
    setLabelText(message.ToQString());
}

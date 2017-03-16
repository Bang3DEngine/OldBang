#include "GameBuildDialog.h"

#include "Debug.h"
#include "GameBuilder.h"
#include "EditorWindow.h"

GameBuildDialog::GameBuildDialog() :
    QProgressDialog("Building game...",
                    "Cancel", 0, 100)
{
    setModal(true);
    setWindowTitle("Building game...");
    setWindowModality(Qt::WindowModality::ApplicationModal);
    QObject::connect(this, SIGNAL(canceled()), SLOT(OnCanceled()));
    show();
    raise();
    activateWindow();
}

void GameBuildDialog::Destroy()
{
    close();
}

void GameBuildDialog::SetPercent(float percent)
{
    setValue(percent * 100);
}

void GameBuildDialog::SetMessage(const QString &message)
{
    setLabelText(message);
}

void GameBuildDialog::OnCanceled()
{
    GameBuilder::GetInstance()->OnGameBuildingHasBeenCanceled();
}

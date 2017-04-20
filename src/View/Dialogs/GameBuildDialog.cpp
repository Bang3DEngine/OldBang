#include "Bang/GameBuildDialog.h"

#include "Bang/Debug.h"
#include "Bang/GameBuilder.h"
#include "Bang/EditorWindow.h"

GameBuildDialog::GameBuildDialog() :
    QProgressDialog("Building game...", "Cancel", 0, 100)
{
    setFixedSize(350, 80);
    setSizeGripEnabled(false);

    setModal(true);
    setWindowFlags(Qt::Dialog);
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

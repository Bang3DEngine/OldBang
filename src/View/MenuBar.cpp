#include "MenuBar.h"

#include "WindowEventManager.h"
#include "WindowMain.h"


#include "Canvas.h"
#include "Persistence.h"
#include "EditorStage.h"
#include "StageReader.h"
#include "FileWriter.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    WindowMain *w = WindowMain::GetInstance();
    connect(w->actionOpenStage,  SIGNAL(triggered()), this, SLOT(OnOpenStage()));
    connect(w->actionSaveStageAs,  SIGNAL(triggered()), this, SLOT(OnSaveStageAs()));

    connect(w->actionCreateEmptyEntity,  SIGNAL(triggered()), this, SLOT(OnCreateEmptyEntity()));
    connect(w->actionCreatePrefab,  SIGNAL(triggered()), this, SLOT(OnCreatePrefab()));
    connect(w->actionCreateMaterial,  SIGNAL(triggered()), this, SLOT(OnCreateMaterial()));
    connect(w->actionCreateMesh,  SIGNAL(triggered()), this, SLOT(OnCreateMesh()));
    connect(w->actionCreateShaderProgram,  SIGNAL(triggered()), this, SLOT(OnCreateShaderProgram()));
    connect(w->actionCreateTexture2D,  SIGNAL(triggered()), this, SLOT(OnCreateTexture2D()));

    connect(w->actionAddPartBehaviour,  SIGNAL(triggered()), this, SLOT(OnAddPartBehaviour()));
    connect(w->actionAddPartCamera,  SIGNAL(triggered()), this, SLOT(OnAddPartCamera()));
    connect(w->actionAddPartMeshRenderer,  SIGNAL(triggered()), this, SLOT(OnAddPartMeshRenderer()));
    connect(w->actionAddPartTransform,  SIGNAL(triggered()), this, SLOT(OnAddPartTransform()));
}

void MenuBar::OnOpenStage() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::OpenStage);

    std::string filename =
        QFileDialog::getOpenFileName(
                        WindowMain::GetMainWindow(),
                        QString::fromStdString("Open stage"),
                        QString::fromStdString(Persistence::GetAssetsPathAbsolute()))
            .toStdString();
    if(filename == "") return;

    EditorStage *stage = new EditorStage();
    StageReader::ReadStage(filename, stage);
    if(stage != nullptr)
    {
        Canvas::GetInstance()->AddStage(stage);
        Canvas::GetInstance()->SetCurrentStage(stage);
    }
    else
    {
        Logger_Error("Stage from file '" << filename << "' could not be loaded.");
    }
}

void MenuBar::OnSaveStageAs() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::SaveCurrentStage);


    Stage *stage = Canvas::GetInstance()->GetCurrentStage();
    if(stage == nullptr) return;

    std::string filename =
        QFileDialog::getSaveFileName(
                WindowMain::GetMainWindow(),
                QString::fromStdString("Save stage as..."),
                QString::fromStdString(Persistence::GetAssetsPathAbsolute()))
            .toStdString();
    if(filename == "") return;
    FileWriter::WriteStage(filename, stage);
}

void MenuBar::OnCreateEmptyEntity() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateEmptyEntity);
}
void MenuBar::OnCreatePrefab() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreatePrefab);
}
void MenuBar::OnCreateMaterial() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateMaterial);
}
void MenuBar::OnCreateMesh() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateMesh);
}
void MenuBar::OnCreateShaderProgram() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateShaderProgram);
}
void MenuBar::OnCreateTexture2D() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateTexture2D);
}


void MenuBar::OnAddPartBehaviour() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartBehaviour);
}
void MenuBar::OnAddPartCamera() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartCamera);
}
void MenuBar::OnAddPartMeshRenderer() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartMeshRenderer);
}
void MenuBar::OnAddPartTransform() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartTransform);
}

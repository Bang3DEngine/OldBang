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
    connect(w->actionNewStage,  SIGNAL(triggered()), this, SLOT(OnNewStage()));
    connect(w->actionOpenStage,  SIGNAL(triggered()), this, SLOT(OnOpenStage()));
    connect(w->actionSaveStage,  SIGNAL(triggered()), this, SLOT(OnSaveStage()));
    connect(w->actionSaveStageAs,  SIGNAL(triggered()), this, SLOT(OnSaveStageAs()));

    connect(w->actionCreateEmptyEntity,  SIGNAL(triggered()), this, SLOT(OnCreateEmptyEntity()));
    connect(w->actionCreateFromPrefab,  SIGNAL(triggered()), this, SLOT(OnCreateFromPrefab()));

    connect(w->actionCreatePrefab,  SIGNAL(triggered()), this, SLOT(OnCreatePrefab()));
    connect(w->actionCreateMaterial,  SIGNAL(triggered()), this, SLOT(OnCreateMaterial()));
    connect(w->actionCreateMesh,  SIGNAL(triggered()), this, SLOT(OnCreateMesh()));
    connect(w->actionCreateShaderProgram,  SIGNAL(triggered()), this, SLOT(OnCreateShaderProgram()));
    connect(w->actionCreateTexture2D,  SIGNAL(triggered()), this, SLOT(OnCreateTexture2D()));

    connect(w->actionAddPartBehaviour,  SIGNAL(triggered()), this, SLOT(OnAddPartBehaviour()));
    connect(w->actionAddPartCamera,  SIGNAL(triggered()), this, SLOT(OnAddPartCamera()));
    connect(w->actionAddPartMeshRenderer,  SIGNAL(triggered()), this, SLOT(OnAddPartMeshRenderer()));
    connect(w->actionAddPartLineRenderer,  SIGNAL(triggered()), this, SLOT(OnAddPartLineRenderer()));
    connect(w->actionAddPartTransform,  SIGNAL(triggered()), this, SLOT(OnAddPartTransform()));
}



void MenuBar::CreateNewStage() const
{
    Stage *stage = new EditorStage();
    Canvas::GetInstance()->SetCurrentStage(stage);
    Persistence::SetCurrentStageFilepath("");
}

QMessageBox::StandardButton MenuBar::AskForSavingCurrentStage() const
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(WindowMain::GetMainWindow(),
                                  "Save Stage",
                                  "Do you want to save your current Stage?",
                                  (QMessageBox::Yes |
                                   QMessageBox::No |
                                   QMessageBox::Cancel
                                   )
                                  );

    if (reply == QMessageBox::Yes)
    {
        OnSaveStage();
    }

    return reply;
}




void MenuBar::OnNewStage() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::NewStage);

    if(AskForSavingCurrentStage() == QMessageBox::Cancel) return;
    CreateNewStage();
}

void MenuBar::OnOpenStage() const
{
    if(AskForSavingCurrentStage() == QMessageBox::Cancel) return;

    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::OpenStage);

    std::string ext = Stage::GetFileExtension();
    std::string filename =
        QFileDialog::getOpenFileName
            (
                        WindowMain::GetMainWindow(),
                        QString::fromStdString("Open stage..."),
                        QString::fromStdString(Persistence::GetAssetsPathAbsolute()),
                        QString::fromStdString( ext + "(*." + ext + ")" )
                        )
            .toStdString();
    if(filename == "") return;


    EditorStage *stage = new EditorStage();
    StageReader::ReadStage(filename, stage);
    if(stage != nullptr)
    {
        Canvas::GetInstance()->AddStage(stage);
        Canvas::GetInstance()->SetCurrentStage(stage);
        Persistence::SetCurrentStageFilepath(filename);
    }
    else
    {
        Logger_Error("Stage from file '" << filename << "' could not be loaded.");
    }
}

void MenuBar::OnSaveStage() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::SaveStage);

    std::string filename = Persistence::GetCurrentStageFilepath();
    if( filename == "" ) //Give the stage a name
    {
        OnSaveStageAs();
    }
    else //Save directly
    {
        Stage *stage = Canvas::GetInstance()->GetCurrentStage();
        if(stage == nullptr) return;
        FileWriter::WriteStage(filename, stage);
    }
}

void MenuBar::OnSaveStageAs() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::SaveStageAs);

    Stage *stage = Canvas::GetInstance()->GetCurrentStage();
    if(stage == nullptr) return;

    std::string ext = Stage::GetFileExtension();
    std::string filename =
        QFileDialog::getSaveFileName
            (
                WindowMain::GetMainWindow(),
                QString::fromStdString("Save stage as..."),
                QString::fromStdString(Persistence::GetAssetsPathAbsolute()),
                QString::fromStdString( ext + "(*." + ext + ")" )
            )
            .toStdString();
    if(filename == "") return;

    FileWriter::WriteStage(filename, stage);
}

void MenuBar::OnCreateEmptyEntity() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateEmptyEntity);
}
void MenuBar::OnCreateFromPrefab() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::CreateFromPrefab);

    std::string ext = Prefab::GetFileExtensionStatic();
    std::string filename =
        QFileDialog::getOpenFileName
            (
                        WindowMain::GetMainWindow(),
                        QString::fromStdString("Create from prefab..."),
                        QString::fromStdString(Persistence::GetAssetsPathAbsolute()),
                        QString::fromStdString( ext + "(*." + ext + ")" )
            )
            .toStdString();
    if(filename == "") return;

    Logger_Log("OSTIA HAHAHAHHAHA");
    std::fstream f;
    f.open(filename);
    if(f.is_open())
    {
        Prefab *p = new Prefab();
        Logger_Log("HIAHIA");
        p->Read(f);
        Logger_Log("WOLOLITO");
        Entity *e = p->Instantiate();
        Logger_Log("holitaaa");
        Entity *selectedEntity = WindowMain::GetInstance()->widgetHierarchy->
                GetFirstSelectedEntity();
        Logger_Log("ESTUPIDO");
        if(selectedEntity != nullptr)
        {
            selectedEntity->AddChild(e);
            Logger_Log("ENGREIDO");
        }
        else
        {
            Stage *currentStage = Canvas::GetInstance()->GetCurrentStage();
            if(currentStage != nullptr)
            {
                Logger_Log("MEC");
                currentStage->AddChild(e);
                Logger_Log("MOC");
            }
        }
        Logger_Log("KA");
        delete p;
        Logger_Log("KKI");
    }
    else
    {
        Logger_Error("Prefab file '" << filename << "' is corrupt or can't be read.");
    }
    Logger_Log("DONEEEE");
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
void MenuBar::OnAddPartLineRenderer() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartLineRenderer);
}
void MenuBar::OnAddPartTransform() const
{
    WindowEventManager::GetInstance()->NotifyMenuBarActionClicked(Action::AddPartTransform);
}

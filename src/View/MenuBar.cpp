#include "MenuBar.h"

#include "Canvas.h"
#include "WindowMain.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "FileDialog.h"
#include "SystemUtils.h"
#include "Persistence.h"
#include "EditorScene.h"
#include "WindowEventManager.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    m_wem = WindowEventManager::GetInstance();

    WindowMain *w = WindowMain::GetInstance();

    connect(w->actionNewScene,  SIGNAL(triggered()),
            this, SLOT(OnNewScene()));
    connect(w->actionOpenScene,  SIGNAL(triggered()),
            this, SLOT(OnOpenScene()));
    connect(w->actionSaveScene,  SIGNAL(triggered()),
            this, SLOT(OnSaveScene()));
    connect(w->actionSaveSceneAs,  SIGNAL(triggered()),
            this, SLOT(OnSaveSceneAs()));
    connect(w->actionBuild,  SIGNAL(triggered()),
            this, SLOT(OnBuild()));
    connect(w->actionBuildAndRun,  SIGNAL(triggered()),
            this, SLOT(OnBuildAndRun()));


    connect(w->actionCreateEmptyGameObject,  SIGNAL(triggered()),
            this, SLOT(OnCreateEmptyGameObject()));
    connect(w->actionCreateFromPrefab,  SIGNAL(triggered()),
            this, SLOT(OnCreateFromPrefab()));
    connect(w->actionCreatePlane,  SIGNAL(triggered()),
            this, SLOT(OnCreatePlane()));
    connect(w->actionCreateCube,  SIGNAL(triggered()),
            this, SLOT(OnCreateCube()));
    connect(w->actionCreateSphere,  SIGNAL(triggered()),
            this, SLOT(OnCreateSphere()));

    connect(w->actionCreatePrefab,  SIGNAL(triggered()),
            this, SLOT(OnCreatePrefab()));
    connect(w->actionCreateMaterial,  SIGNAL(triggered()),
            this, SLOT(OnCreateMaterial()));
    connect(w->actionCreateMesh,  SIGNAL(triggered()),
            this, SLOT(OnCreateMesh()));
    connect(w->actionCreateShaderProgram,  SIGNAL(triggered()),
            this, SLOT(OnCreateShaderProgram()));
    connect(w->actionCreateTexture2D,  SIGNAL(triggered()),
            this, SLOT(OnCreateTexture2D()));

    connect(w->actionAddComponentBehaviour,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentBehaviour()));
    connect(w->actionAddComponentCamera,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentCamera()));
    connect(w->actionAddComponentTransform,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentTransform()));
    connect(w->actionAddComponentMeshRenderer,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentMeshRenderer()));
    connect(w->actionAddComponentLineRenderer,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentSingleLineRenderer()));
    connect(w->actionAddComponentCircleRenderer,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentCircleRenderer()));
    connect(w->actionAddComponentDirectionalLight,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentDirectionalLight()));
    connect(w->actionAddComponentPointLight,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentPointLight()));
}



void MenuBar::CreateNewScene() const
{
    Scene *scene = new EditorScene();
    Canvas::GetInstance()->SetCurrentScene(scene);
    Persistence::SetCurrentSceneFilepath("");
}

QMessageBox::StandardButton MenuBar::AskForSavingCurrentScene() const
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(WindowMain::GetInstance()->GetMainWindow(),
                                  "Save Scene",
                                  "Do you want to save your current Scene?",
                                  (QMessageBox::Yes |  QMessageBox::No |
                                   QMessageBox::Cancel )
                                  );

    if (reply == QMessageBox::Yes)
    {
        OnSaveScene();
    }

    return reply;
}




void MenuBar::OnNewScene() const
{
    m_wem->NotifyMenuBarActionClicked(Action::NewScene);

    if (AskForSavingCurrentScene() == QMessageBox::Cancel) return;
    CreateNewScene();
}

void MenuBar::OnOpenScene() const
{
    if (AskForSavingCurrentScene() == QMessageBox::Cancel) return;

    m_wem->NotifyMenuBarActionClicked(Action::OpenScene);

    FileDialog fd("Open scene...", Scene::GetFileExtension());
    std::string filename = fd.GetOpenFilename();
    if (filename == "") return;

    EditorScene *scene = new EditorScene();
    FileReader::ReadScene(filename, scene);
    if (scene )
    {
        Canvas::GetInstance()->AddScene(scene);
        Canvas::GetInstance()->SetCurrentScene(scene);
        Persistence::SetCurrentSceneFilepath(filename);
    }
    else
    {
        Logger_Error("Scene from file '" << filename <<
                     "' could not be loaded.");
    }
}

void MenuBar::OnSaveScene() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveScene);

    std::string filename = Persistence::GetCurrentSceneFilepath();
    if ( filename == "" ) //Give the scene a name
    {
        OnSaveSceneAs();
    }
    else //Save directly
    {
        Scene *scene = Canvas::GetInstance()->GetCurrentScene(); NONULL(scene);
        FileWriter::WriteScene(filename, scene);
    }
}

void MenuBar::OnSaveSceneAs() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveSceneAs);

    Scene *scene = Canvas::GetInstance()->GetCurrentScene(); NONULL(scene);

    FileDialog fd("Save scene as...", Scene::GetFileExtension());
    std::string filename = fd.GetSaveFilename(scene->name);
    if (filename == "") return;

    FileWriter::WriteScene(filename, scene);
}

void MenuBar::OnBuild() const
{
    Logger_Log("Building Game... (This could take a while)");
    GameBuilder::BuildGame(Persistence::GetProjectRootPathAbsolute());
}

void MenuBar::OnBuildAndRun() const
{
    Logger_Log("Building and running Game... (This could take a while)");
    GameBuilder::BuildAndRunGame(Persistence::GetProjectRootPathAbsolute());
}


void MenuBar::OnCreateEmptyGameObject() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateEmptyGameObject);
}
void MenuBar::OnCreateFromPrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateFromPrefab);

    FileDialog fd("Create from prefab...", Prefab::GetFileExtensionStatic());
    std::string filename = fd.GetOpenFilename();
    if (filename == "") return;

    std::fstream f;
    f.open(filename);
    if (f.is_open())
    {
        WindowMain *w = WindowMain::GetInstance();

        Prefab *p = new Prefab();
        p->ReadInternal(f);
        GameObject *e = p->InstantiateWithoutStarting();
        GameObject *selectedGameObject = w->widgetHierarchy->GetFirstSelectedGameObject();

        if (selectedGameObject )
        {
            e->SetParent(selectedGameObject);
        }
        else
        {
            Scene *currentScene = Canvas::GetInstance()->GetCurrentScene();
            if (currentScene )
            {
                e->SetParent(currentScene);
            }
        }
        delete p;

        Hierarchy::GetInstance()->SelectGameObject(e);
    }
    else
    {
        Logger_Error("Prefab file '" << filename <<
                     "' is corrupt or can't be read.");
    }
}


GameObject* MenuBar::CreatePrimitiveGameObject(Mesh *m, const std::string &name) const
{
    GameObject *go = MeshFactory::CreatePrimitiveGameObject(m, name);

    go->SetParent(Canvas::GetInstance()->GetCurrentScene());
    Hierarchy::GetInstance()->SelectGameObject(go);
    return go;
}

void MenuBar::OnCreatePlane() const
{
    MenuBar::CreatePrimitiveGameObject(MeshFactory::GetPlane(), "Plane");
}

void MenuBar::OnCreateCube() const
{
    MenuBar::CreatePrimitiveGameObject(MeshFactory::GetCube(), "Cube");
}

void MenuBar::OnCreateSphere() const
{
    MenuBar::CreatePrimitiveGameObject(MeshFactory::GetSphere(), "Sphere");
}

void MenuBar::OnCreateCone() const
{
    MenuBar::CreatePrimitiveGameObject(MeshFactory::GetCone(), "Cone");
}

void MenuBar::OnCreatePrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreatePrefab);
}
void MenuBar::OnCreateMaterial() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMaterial);
}
void MenuBar::OnCreateMesh() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMesh);
}
void MenuBar::OnCreateShaderProgram() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateShaderProgram);
}
void MenuBar::OnCreateTexture2D() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateTexture2D);
}


void MenuBar::OnAddComponentBehaviour() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentBehaviour);
}
void MenuBar::OnAddComponentCamera() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentCamera);
}
void MenuBar::OnAddComponentTransform() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentTransform);
}
void MenuBar::OnAddComponentMeshRenderer() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentMeshRenderer);
}
void MenuBar::OnAddComponentSingleLineRenderer() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponenSingleLineRenderer);
}
void MenuBar::OnAddComponentCircleRenderer() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentCircleRenderer);
}

void MenuBar::OnAddComponentDirectionalLight() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentDirectionalLight);
}

void MenuBar::OnAddComponentPointLight() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentPointLight);
}

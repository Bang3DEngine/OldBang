#include "MenuBar.h"

#include "WindowEventManager.h"
#include "WindowMain.h"

#include "Canvas.h"
#include "Persistence.h"
#include "EditorScene.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "FileDialog.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    wem = WindowEventManager::GetInstance();

    WindowMain *w = WindowMain::GetInstance();
    connect(w->actionNewScene,  SIGNAL(triggered()),
            this, SLOT(OnNewScene()));
    connect(w->actionOpenScene,  SIGNAL(triggered()),
            this, SLOT(OnOpenScene()));
    connect(w->actionSaveScene,  SIGNAL(triggered()),
            this, SLOT(OnSaveScene()));
    connect(w->actionSaveSceneAs,  SIGNAL(triggered()),
            this, SLOT(OnSaveSceneAs()));

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
            QMessageBox::question(WindowMain::GetMainWindow(),
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
    wem->NotifyMenuBarActionClicked(Action::NewScene);

    if(AskForSavingCurrentScene() == QMessageBox::Cancel) return;
    CreateNewScene();
}

void MenuBar::OnOpenScene() const
{
    if(AskForSavingCurrentScene() == QMessageBox::Cancel) return;

    wem->NotifyMenuBarActionClicked(Action::OpenScene);

    FileDialog fd("Open scene...", Scene::GetFileExtension());
    std::string filename = fd.GetOpenFilename();
    if(filename == "") return;

    EditorScene *scene = new EditorScene();
    FileReader::ReadScene(filename, scene);
    if(scene != nullptr)
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
    wem->NotifyMenuBarActionClicked(Action::SaveScene);

    std::string filename = Persistence::GetCurrentSceneFilepath();
    if( filename == "" ) //Give the scene a name
    {
        OnSaveSceneAs();
    }
    else //Save directly
    {
        Scene *scene = Canvas::GetInstance()->GetCurrentScene();
        if(scene == nullptr) return;
        FileWriter::WriteScene(filename, scene);
    }
}

void MenuBar::OnSaveSceneAs() const
{
    wem->NotifyMenuBarActionClicked(Action::SaveSceneAs);

    Scene *scene = Canvas::GetInstance()->GetCurrentScene();
    if(scene == nullptr) return;

    FileDialog fd("Save scene as...", Scene::GetFileExtension());
    std::string filename = fd.GetSaveFilename(scene->GetName());
    if(filename == "") return;

    FileWriter::WriteScene(filename, scene);
}

void MenuBar::OnCreateEmptyGameObject() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateEmptyGameObject);
}
void MenuBar::OnCreateFromPrefab() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateFromPrefab);

    FileDialog fd("Create from prefab...", Prefab::GetFileExtensionStatic());
    std::string filename = fd.GetOpenFilename();
    if(filename == "") return;

    std::fstream f;
    f.open(filename);
    if(f.is_open())
    {
        Prefab *p = new Prefab();
        p->Read(f);
        GameObject *e = p->InstantiateWithoutStarting();
        GameObject *selectedGameObject = WindowMain::GetInstance()->
                widgetHierarchy->GetFirstSelectedGameObject();

        if(selectedGameObject != nullptr)
        {
            selectedGameObject->AddChild(e);
        }
        else
        {
            Scene *currentScene = Canvas::GetInstance()->GetCurrentScene();
            if(currentScene != nullptr)
            {
                currentScene->AddChild(e);
            }
        }
        delete p;
    }
    else
    {
        Logger_Error("Prefab file '" << filename <<
                     "' is corrupt or can't be read.");
    }
}

void MenuBar::OnCreatePlane() const
{
    // wem->NotifyMenuBarActionClicked(Action::CreatePlane);

    GameObject *go = new GameObject();
    go->AddComponent<Transform>();

    Mesh *m = new Mesh();
    std::vector<Vector3> pos, normals; std::vector<glm::vec2> uvs;
    MeshFactory::GetPlaneTris(1.0f, pos, normals, uvs);
    m->LoadPositions(pos);
    m->LoadNormals(normals);
    m->LoadUvs(uvs);

    Material *mat = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/default.bmat");

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(MeshRenderer::RenderMode::Triangles);
    r->SetMaterial(mat);
    r->SetMesh(m);

    go->SetName("Plane");
    Canvas::GetInstance()->GetCurrentScene()->AddChild(go);
}

void MenuBar::OnCreateCube() const
{
    // wem->NotifyMenuBarActionClicked(Action::CreateCube);

    GameObject *go = new GameObject();
    go->AddComponent<Transform>();

    Mesh *m = new Mesh();
    std::vector<Vector3> pos, normals; std::vector<glm::vec2> uvs;
    MeshFactory::GetCubeTris(1.0f, pos, normals, uvs);
    m->LoadPositions(pos);
    m->LoadNormals(normals);
    m->LoadUvs(uvs);

    Material *mat = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/default.bmat");

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(MeshRenderer::RenderMode::Triangles);
    r->SetMaterial(mat);
    r->SetMesh(m);

    go->SetName("Cube");
    Canvas::GetInstance()->GetCurrentScene()->AddChild(go);
}

void MenuBar::OnCreateSphere() const
{
    // wem->NotifyMenuBarActionClicked(Action::CreateSphere);
}

void MenuBar::OnCreatePrefab() const
{
    wem->NotifyMenuBarActionClicked(Action::CreatePrefab);
}
void MenuBar::OnCreateMaterial() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateMaterial);
}
void MenuBar::OnCreateMesh() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateMesh);
}
void MenuBar::OnCreateShaderProgram() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateShaderProgram);
}
void MenuBar::OnCreateTexture2D() const
{
    wem->NotifyMenuBarActionClicked(Action::CreateTexture2D);
}


void MenuBar::OnAddComponentBehaviour() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponentBehaviour);
}
void MenuBar::OnAddComponentCamera() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponentCamera);
}
void MenuBar::OnAddComponentTransform() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponentTransform);
}
void MenuBar::OnAddComponentMeshRenderer() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponentMeshRenderer);
}
void MenuBar::OnAddComponentSingleLineRenderer() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponenSingleLineRenderer);
}
void MenuBar::OnAddComponentCircleRenderer() const
{
    wem->NotifyMenuBarActionClicked(Action::AddComponentCircleRenderer);
}

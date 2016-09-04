#include "MenuBar.h"

#include "Screen.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "FileDialog.h"
#include "PointLight.h"
#include "SystemUtils.h"
#include "Persistence.h"
#include "EditorScene.h"
#include "Application.h"
#include "SceneManager.h"
#include "EditorWindow.h"
#include "DirectionalLight.h"
#include "WindowEventManager.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    m_wem = WindowEventManager::GetInstance();

    EditorWindow *w = EditorWindow::GetInstance();

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
    connect(w->actionCreateDirectionalLight,  SIGNAL(triggered()),
            this, SLOT(OnCreateDirectionalLight()));
    connect(w->actionCreatePointLight,  SIGNAL(triggered()),
            this, SLOT(OnCreatePointLight()));
    connect(w->actionAlignGameObjectWithView,  SIGNAL(triggered()),
            this, SLOT(OnAlignGameObjectWithView()));
    connect(w->actionAlignViewWithGameObject,  SIGNAL(triggered()),
            this, SLOT(OnAlignViewWithGameObject()));

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
    SceneManager::SetActiveScene(nullptr);
    Scene *scene = new EditorScene();
    SceneManager::SetActiveScene(scene);
    Persistence::SetActiveSceneFilepath("");
}

QMessageBox::StandardButton MenuBar::AskForSavingActiveScene() const
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(EditorWindow::GetInstance()->GetMainWindow(),
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
    if (AskForSavingActiveScene() == QMessageBox::Cancel) return;
    CreateNewScene();
}

void MenuBar::OnOpenScene() const
{
    if (AskForSavingActiveScene() == QMessageBox::Cancel) return;

    m_wem->NotifyMenuBarActionClicked(Action::OpenScene);

    FileDialog fd("Open scene...", Scene::GetFileExtension());
    String filename = fd.GetOpenFilename();
    if (filename == "") return;

    SceneManager::SetActiveScene(nullptr);
    EditorScene *scene = new EditorScene();
    FileReader::ReadScene(filename, scene);
    if (scene)
    {
        SceneManager::AddScene(scene);
        SceneManager::SetActiveScene(scene);
        Persistence::SetActiveSceneFilepath(filename);
    }
    else
    {
        Debug_Error("Scene from file '" << filename <<
                     "' could not be loaded.");
    }
}

void MenuBar::OnSaveScene() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveScene);

    String filename = Persistence::SetActiveSceneFilepath();
    if ( filename == "" ) //Give the scene a name
    {
        OnSaveSceneAs();
    }
    else //Save directly
    {
        Scene *scene = SceneManager::GetActiveScene(); NONULL(scene);
        FileWriter::WriteScene(filename, scene);
    }
}

void MenuBar::OnSaveSceneAs() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveSceneAs);

    Scene *scene = SceneManager::GetActiveScene(); NONULL(scene);

    FileDialog fd("Save scene as...", Scene::GetFileExtension());
    String filename = fd.GetSaveFilename(scene->name);
    if (filename == "") return;

    FileWriter::WriteScene(filename, scene);
}

void MenuBar::OnBuild() const
{
    Debug_Log("Building Game... (This could take a while)");
    GameBuilder::BuildGame(Persistence::GetProjectRootPathAbsolute(), false);
}

void MenuBar::OnBuildAndRun() const
{
    Debug_Log("Building and running Game... (This could take a while)");
    GameBuilder::BuildGame(Persistence::GetProjectRootPathAbsolute(), true);
}


void MenuBar::OnCreateEmptyGameObject() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateEmptyGameObject);
}
void MenuBar::OnCreateFromPrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateFromPrefab);

    FileDialog fd("Create from prefab...", Prefab::GetFileExtensionStatic());
    String filename = fd.GetOpenFilename();
    if (filename == "") { return; }

    EditorWindow *w = EditorWindow::GetInstance();

    Prefab *p = new Prefab();

    XMLNode *xmlInfo = XMLParser::FromFile(filename);
    if (xmlInfo)
    {
        p->ReadXMLInfo(xmlInfo);
        delete xmlInfo;

        GameObject *go = p->InstantiateWithoutStarting();
        GameObject *selectedGameObject = w->widgetHierarchy->GetFirstSelectedGameObject();

        if (selectedGameObject )
        {
            go->SetParent(selectedGameObject);
        }
        else
        {
            Scene *activeScene = SceneManager::GetActiveScene();
            if (activeScene)
            {
                go->SetParent(activeScene);
            }
        }
        delete p;

        Hierarchy::GetInstance()->SelectGameObject(go);
    }
    else
    {
        Debug_Error("Prefab file '" << filename << "' can't be read.");
    }
}


GameObject* MenuBar::CreatePrimitiveGameObject(Mesh *m, const String &name) const
{
    GameObject *go = MeshFactory::CreatePrimitiveGameObject(m, name);

    go->SetParent(SceneManager::GetActiveScene());
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

void MenuBar::OnCreateDirectionalLight() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateDirectionalLight);
    GameObject *go = new GameObject("DirectionalLight");
    go->SetParent(SceneManager::GetActiveScene());
    go->AddComponent<DirectionalLight>();
    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnCreatePointLight() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreatePointLight);
    GameObject *go = new GameObject("PointLight");
    go->SetParent(SceneManager::GetActiveScene());
    go->AddComponent<PointLight>();
    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnAlignGameObjectWithView() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AlignGameObjectWithView);

    GameObject *selected = Hierarchy::GetInstance()->GetFirstSelectedGameObject();
    if (selected)
    {
        EditorCamera *editorCamera =
                static_cast<EditorCamera*>(Scene::GetCamera()->gameObject->parent);
        selected->transform->SetPosition(editorCamera->transform->GetPosition());
        selected->transform->SetRotation(editorCamera->transform->GetRotation());
    }
}

void MenuBar::OnAlignViewWithGameObject() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AlignViewWithGameObject);

    GameObject *selected = Hierarchy::GetInstance()->GetFirstSelectedGameObject();
    if (selected)
    {
        EditorCamera *editorCamera =
                static_cast<EditorCamera*>(Scene::GetCamera()->gameObject->parent);
        editorCamera->AlignViewWithGameObject(selected);
    }

}

void MenuBar::OnCreatePrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreatePrefab);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Prefab." + Prefab::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath, Explorer::GetInstance());
    AssetsManager::CreateAsset<Prefab>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateMaterial() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMaterial);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Material." + Material::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath, Explorer::GetInstance());
    AssetsManager::CreateAsset<Material>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateMesh() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMesh);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Mesh." + Mesh::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath, Explorer::GetInstance());
    AssetsManager::CreateAsset<Mesh>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateShaderProgram() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateShaderProgram);
    /*
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_ShaderProgram." + ShaderProgram::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath, Explorer::GetInstance());
    AssetsManager::CreateAsset<ShaderProgram>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
    */
}
void MenuBar::OnCreateTexture2D() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateTexture2D);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Texture2D." + Texture2D::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath, Explorer::GetInstance());
    AssetsManager::CreateAsset<Texture2D>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}


void MenuBar::OnAddComponentBehaviour() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentBehaviour);
}
void MenuBar::OnAddComponentCamera() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentCamera);
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

#include "MenuBar.h"

#include "Font.h"
#include "Mesh.h"
#include "Debug.h"
#include "Screen.h"
#include "Prefab.h"
#include "Camera.h"
#include "Project.h"
#include "Material.h"
#include "Transform.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "PointLight.h"
#include "MeshFactory.h"
#include "SystemUtils.h"
#include "Persistence.h"
#include "EditorScene.h"
#include "Application.h"
#include "GameBuilder.h"
#include "EditorCamera.h"
#include "SceneManager.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "ProjectManager.h"
#include "DirectionalLight.h"
#include "WindowEventManager.h"

MenuBar *MenuBar::s_instance = nullptr;

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    MenuBar::s_instance = this;
    m_wem = WindowEventManager::GetInstance();

    EditorWindow *w = EditorWindow::GetInstance();

    connect(w->actionNewProject,  SIGNAL(triggered()),
            this, SLOT(OnNewProject()));
    connect(w->actionOpenProject,  SIGNAL(triggered()),
            this, SLOT(OnOpenProject()));
    connect(w->actionSaveProject,  SIGNAL(triggered()),
            this, SLOT(OnSaveProject()));
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
    connect(w->actionCreateFont,  SIGNAL(triggered()),
            this, SLOT(OnCreateFont()));

    connect(w->actionAddComponentNewBehaviour,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentNewBehaviour()));
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
    connect(w->actionAddComponentCanvas,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentCanvas()));
    connect(w->actionAddComponentUIImage,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentUIImage()));
    connect(w->actionAddComponentUIText,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentUIText()));
}

void MenuBar::CreateNewScene() const
{
    SceneManager::SetActiveScene(nullptr);
    Scene *scene = Scene::GetDefaultScene();
    SceneManager::SetActiveScene(scene);
    Persistence::SetActiveSceneFilepath("");
}

Dialog::Reply MenuBar::AskForSavingActiveScene() const
{
    Dialog::Reply reply = Dialog::GetYesNoCancel(
                "Save Scene",
                "Do you want to save the current Scene?");

    if (reply == Dialog::Reply::Yes)
    {
        OnSaveScene();
    }
    return reply;
}

void MenuBar::OnNewProject() const
{
    String projectFilepath = ProjectManager::DialogCreateNewProject();
    ProjectManager::OpenProject(projectFilepath);
}

void MenuBar::OnOpenProject() const
{
    String projectFilepath = ProjectManager::DialogOpenProject();
    ProjectManager::OpenProject(projectFilepath);
}

void MenuBar::OnSaveProject() const
{
    ProjectManager::SaveProject( ProjectManager::GetCurrentProject() );
}


void MenuBar::OnNewScene() const
{
    m_wem->NotifyMenuBarActionClicked(Action::NewScene);
    if (AskForSavingActiveScene() == Dialog::Reply::Cancel) return;
    CreateNewScene();
}

void MenuBar::OnOpenScene() const
{
    if (AskForSavingActiveScene() == Dialog::Reply::Cancel) return;

    m_wem->NotifyMenuBarActionClicked(Action::OpenScene);

    String filename = Dialog::GetOpenFilename("Open scene...",
                                              Scene::GetFileExtensionStatic());
    ASSERT(!filename.Empty());

    SceneManager::LoadSceneInstantly(filename);
}

void MenuBar::OnSaveScene() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveScene);

    String filename = Persistence::GetCurrentSceneFilepath();
    if ( filename == "" ) //Give the scene a name
    {
        OnSaveSceneAs();
    }
    else //Save directly
    {
        Scene *scene = SceneManager::GetActiveScene(); ASSERT(scene);
        FileWriter::WriteScene(filename, scene);
    }
}

void MenuBar::OnSaveSceneAs() const
{
    m_wem->NotifyMenuBarActionClicked(Action::SaveSceneAs);

    Scene *scene = SceneManager::GetActiveScene(); ASSERT(scene);

    String filename = Dialog::GetSaveFilename("Save scene as...",
                                              Scene::GetFileExtensionStatic(),
                                              Persistence::GetProjectAssetsRootAbs(),
                                              scene->name);
    if (filename == "") return;

    FileWriter::WriteScene(filename, scene);
}

void MenuBar::OnBuild() const
{
    GameBuilder::GetInstance()->BuildGame(false);
}

void MenuBar::OnBuildAndRun() const
{
    GameBuilder::GetInstance()->BuildGame(true);
}


void MenuBar::OnCreateEmptyGameObject() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateEmptyGameObject);
}
void MenuBar::OnCreateFromPrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateFromPrefab);

    String filename = Dialog::GetOpenFilename("Create from prefab...",
                                                  Prefab::GetFileExtensionStatic());
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
                static_cast<EditorCamera*>(SceneManager::GetActiveScene()->GetCamera()->gameObject->parent);
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
                static_cast<EditorCamera*>(SceneManager::GetActiveScene()->GetCamera()->gameObject->parent);
        editorCamera->AlignViewWithGameObject(selected);
    }

}

void MenuBar::OnCreatePrefab() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreatePrefab);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Prefab." + Prefab::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::Create<Prefab>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateMaterial() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMaterial);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Material." + Material::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::Create<Material>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateMesh() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateMesh);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Mesh." + Mesh::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::Create<Mesh>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateShaderProgram() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateShaderProgram);
    /*
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_ShaderProgram." + ShaderProgram::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::CreateAsset<ShaderProgram>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
    */
}
void MenuBar::OnCreateTexture2D() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateTexture2D);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Texture2D." + Texture2D::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::Create<Texture2D>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}

void MenuBar::OnCreateFont() const
{
    m_wem->NotifyMenuBarActionClicked(Action::CreateFont);
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Font." + Font::GetFileExtensionStatic();
    filepath = Persistence::GetDuplicateName(filepath);
    AssetsManager::Create<Font>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}

void MenuBar::OnAddComponentNewBehaviour() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentNewBehaviour);
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
void MenuBar::OnAddComponentCanvas() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentCanvas);
}
void MenuBar::OnAddComponentUIImage() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentUIImage);
}

void MenuBar::OnAddComponentUIText() const
{
    m_wem->NotifyMenuBarActionClicked(Action::AddComponentUIText);
}

MenuBar *MenuBar::GetInstance()
{
    return MenuBar::s_instance;
}

#include "Bang/MenuBar.h"

#include "Bang/IO.h"
#include "Bang/Font.h"
#include "Bang/Mesh.h"
#include "Bang/Debug.h"
#include "Bang/Canvas.h"
#include "Bang/Screen.h"
#include "Bang/Prefab.h"
#include "Bang/Project.h"
#include "Bang/Material.h"
#include "Bang/Inspector.h"
#include "Bang/Texture2D.h"
#include "Bang/FileReader.h"
#include "Bang/MeshFactory.h"
#include "Bang/SystemUtils.h"
#include "Bang/EditorScene.h"
#include "Bang/Application.h"
#include "Bang/GameBuilder.h"
#include "Bang/IInspectable.h"
#include "Bang/EditorCamera.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ProjectManager.h"
#include "Bang/SerializableObject.h"
#include "Bang/WindowEventManager.h"

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
    connect(w->actionCreateUIText,  SIGNAL(triggered()),
            this, SLOT(OnCreateUIText()));
    connect(w->actionCreateUIImage,  SIGNAL(triggered()),
            this, SLOT(OnCreateUIImage()));
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
    connect(w->actionCreateAudioClip,  SIGNAL(triggered()),
            this, SLOT(OnCreateAudioClip()));

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
    connect(w->actionAddComponentAudioSource,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentAudioSource()));
    connect(w->actionAddComponentAudioListener,  SIGNAL(triggered()),
            this, SLOT(OnAddComponentAudioListener()));
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
    connect(w->actionAddComponentPostProcessEffect, SIGNAL(triggered()),
            this, SLOT(OnAddComponentPostProcessEffect()));
}

void MenuBar::CreateNewScene() const
{
    SceneManager::CloseOpenScene();
    SceneManager::SetActiveScene(nullptr);
    Scene *scene = Scene::GetDefaultScene();
    SceneManager::SetActiveScene(scene);
}

Dialog::Reply MenuBar::AskForSavingActiveScene() const
{
    if (SceneManager::IsActiveSceneSaved()) { return Dialog::Reply::No; }

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
    if (AskForSavingActiveScene() == Dialog::Reply::Cancel) return;
    CreateNewScene();
}

void MenuBar::OnOpenScene() const
{
    String filepath = Dialog::GetOpenFilepath("Open scene...",
                                              Scene::GetFileExtensionStatic());
    ENSURE(!filepath.Empty());
    OpenScene(filepath);
}

void MenuBar::OpenScene(const String &filepath) const
{
    if (AskForSavingActiveScene() == Dialog::Reply::Cancel) { return; }
    SceneManager::OpenScene(filepath);
}

void MenuBar::OnSaveScene() const
{
    ENSURE(EditorState::IsStopped());
    String filepath = SceneManager::GetActiveSceneFilepath();
    filepath = IO::AppendExtension(
                           filepath, Scene::GetFileExtensionStatic());

    if (IO::ExistsFile(filepath))
    {
        Scene *scene = SceneManager::GetActiveScene(); ENSURE(scene);
        scene->WriteToFile(filepath);
    }
    else { OnSaveSceneAs(); }
}

void MenuBar::OnSaveSceneAs() const
{
    ENSURE(EditorState::IsStopped());
    Scene *scene = SceneManager::GetActiveScene(); ENSURE(scene);
    String sceneFilepath = SceneManager::GetActiveSceneFilepath();
    String sceneName = IO::GetFileName(sceneFilepath);
    sceneFilepath = Dialog::GetSaveFilepath(
                             "Save scene as...",
                             Scene::GetFileExtensionStatic(),
                             IO::GetProjectAssetsRootAbs(),
                             sceneName);
    ENSURE(!sceneFilepath.Empty());

    sceneFilepath = IO::AppendExtension(sceneFilepath,
                                        Scene::GetFileExtensionStatic());
    SceneManager::OnActiveSceneSavedAs(sceneFilepath);
    scene->WriteToFile(sceneFilepath);
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
    Hierarchy::GetInstance()->OnMenuBarCreateEmptyClicked();
}

void MenuBar::OnCreateFromPrefab() const
{
    String filename = Dialog::GetOpenFilepath("Create from prefab...",
                                              Prefab::GetFileExtensionStatic());
    ENSURE (!filename.Empty());

    EditorWindow *w = EditorWindow::GetInstance();

    Prefab *p = new Prefab();

    XMLNode xmlInfo = XMLParser::FromFile(filename);
    p->Read(xmlInfo);

    GameObject *go = p->InstantiateWithoutStarting();
    GameObject *selectedGameObject =
            w->widgetHierarchy->GetFirstSelectedGameObject();
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
    GameObject *go = new GameObject("DirectionalLight");
    go->SetParent(SceneManager::GetActiveScene());

    go->AddComponent<DirectionalLight>();

    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnCreatePointLight() const
{
    GameObject *go = new GameObject("PointLight");
    go->SetParent(SceneManager::GetActiveScene());

    go->AddComponent<PointLight>();

    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnCreateUIText() const
{
    GameObject *go = new GameObject("Text");
    go->ChangeTransformByRectTransform();
    go->SetParent(SceneManager::GetActiveScene());

    UIText *text = go->AddComponent<UIText>();
    text->SetContent("Bang");

    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnCreateUIImage() const
{
    GameObject *go = new GameObject("Image");
    go->ChangeTransformByRectTransform();
    go->SetParent(SceneManager::GetActiveScene());
    go->ChangeTransformByRectTransform();

    UIImage *img = go->AddComponent<UIImage>();
    String defaultImgPath = "Textures/DefaultUIImageTexture.btex2d";
    Texture2D *defaultTex = AssetsManager::Load<Texture2D>(defaultImgPath, true);
    img->SetImage(defaultTex);

    RectTransform *rt = go->GetComponent<RectTransform>();
    rt->SetAnchors(Vector2(-0.3f), Vector2(0.3f));

    Hierarchy::GetInstance()->SelectGameObject(go);
}

void MenuBar::OnAlignGameObjectWithView() const
{
    GameObject *selected = Hierarchy::GetInstance()->GetFirstSelectedGameObject();
    if (selected)
    {
        EditorCamera *editorCamera =
                SceneManager::GetActiveScene()->
                GetCamera()->gameObject->parent->Cast<EditorCamera>();
        selected->transform->SetPosition(editorCamera->transform->GetPosition());
        selected->transform->SetRotation(editorCamera->transform->GetRotation());
    }
}

void MenuBar::OnAlignViewWithGameObject() const
{
    GameObject *selected = Hierarchy::GetInstance()->GetFirstSelectedGameObject();
    if (selected)
    {
        EditorCamera *editorCamera =
                SceneManager::GetActiveScene()->
                GetCamera()->gameObject->parent->Cast<EditorCamera>();
        editorCamera->AlignViewWithGameObject(selected);
    }

}

void MenuBar::OnCreatePrefab() const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Prefab." + Prefab::GetFileExtensionStatic();
    filepath = IO::GetDuplicatePath(filepath);
    AssetsManager::Create<Prefab>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
Material* MenuBar::OnCreateMaterial(const String &matFilepath) const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/" + IO::AppendExtension(matFilepath,
                                          Material::GetFileExtensionStatic());
    filepath = IO::GetDuplicatePath(filepath);
    Material *mat = AssetsManager::Create<Material>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
    return mat;
}
void MenuBar::OnCreateMesh() const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Mesh." + Mesh::GetFileExtensionStatic();
    filepath = IO::GetDuplicatePath(filepath);
    AssetsManager::Create<Mesh>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
void MenuBar::OnCreateShaderProgram() const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_ShaderProgram." + ShaderProgram::GetFileExtensionStatic();
    filepath = IO::GetDuplicatePath(filepath);
    AssetsManager::Create<ShaderProgram>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}
Texture2D* MenuBar::OnCreateTexture2D(const String &tex2DFilepath) const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/" + IO::AppendExtension(tex2DFilepath,
                                          Texture2D::GetFileExtensionStatic());
    filepath = IO::GetDuplicatePath(filepath);
    Texture2D* tex = AssetsManager::Create<Texture2D>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
    return tex;
}

void MenuBar::OnCreateFont() const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/New_Font." + Font::GetFileExtensionStatic();
    filepath = IO::GetDuplicatePath(filepath);
    AssetsManager::Create<Font>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
}

AudioClip* MenuBar::OnCreateAudioClip(const String &audioClipName) const
{
    String filepath = Explorer::GetInstance()->GetCurrentDir();
    filepath += "/" + IO::AppendExtension(audioClipName,
                                          AudioClip::GetFileExtensionStatic());
    filepath = IO::GetDuplicatePath(filepath);
    AudioClip* audioClip = AssetsManager::Create<AudioClip>(filepath);
    Explorer::GetInstance()->StartRenaming(filepath);
    return audioClip;
}

void MenuBar::OnAddComponentNewBehaviour() const
{
    GameObject* inspectorGameObject = GetInspectorGameObject();
    ENSURE(inspectorGameObject);

    Behaviour *newBehaviour = Behaviour::CreateNewBehaviour();
    OnAddComponent(newBehaviour);
}
void MenuBar::OnAddComponentBehaviour() const
{
    OnAddComponent<Behaviour>();
}
void MenuBar::OnAddComponentCamera() const
{
    OnAddComponent<Camera>();
}
void MenuBar::OnAddComponentMeshRenderer() const
{
    OnAddComponent<MeshRenderer>();
}
void MenuBar::OnAddComponentSingleLineRenderer() const
{
    OnAddComponent<SingleLineRenderer>();
 }
void MenuBar::OnAddComponentCircleRenderer() const
{
    OnAddComponent<CircleRenderer>();
 }
void MenuBar::OnAddComponentAudioSource() const
{
    OnAddComponent<AudioSource>();
 }
void MenuBar::OnAddComponentAudioListener() const
{
    OnAddComponent<AudioListener>();
 }
void MenuBar::OnAddComponentDirectionalLight() const
{
    OnAddComponent<DirectionalLight>();
}
void MenuBar::OnAddComponentPointLight() const
{
    OnAddComponent<PointLight>();
}
void MenuBar::OnAddComponentCanvas() const
{
    OnAddComponent<Canvas>();
}
void MenuBar::OnAddComponentUIImage() const
{
    OnAddComponent<UIImage>();
}
void MenuBar::OnAddComponentUIText() const
{
    OnAddComponent<UIText>();
}

void MenuBar::OnAddComponentPostProcessEffect() const
{
    OnAddComponent<PostProcessEffect>();
}

void MenuBar::OnAddComponent(Component *c) const
{
    ENSURE(c);
    GameObject* inspectorGameObject = GetInspectorGameObject();
    ENSURE(inspectorGameObject);
    inspectorGameObject->AddComponent(c);
}

GameObject *MenuBar::GetInspectorGameObject() const
{
    Inspector *inspector = Inspector::GetInstance();
    if (!inspector) { return nullptr; }

    IInspectable *inspectable = inspector->GetCurrentInspectable();
    if (!inspectable) { return nullptr; }

    SerializableObject *serialObject = inspectable->GetRelatedSerializableObject();
    if (!serialObject) { return nullptr; }

    return Object::Cast<GameObject>(serialObject);
}

MenuBar *MenuBar::GetInstance()
{
    return MenuBar::s_instance;
}

#include "Bang/EditorScene.h"

#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Random.h"
#include "Bang/Camera.h"
#include "Bang/UIText.h"
#include "Bang/Hierarchy.h"
#include "Bang/Transform.h"
#include "Bang/EditorState.h"
#include "Bang/EditorFloor.h"
#include "Bang/EditorCamera.h"
#include "Bang/RectTransform.h"
#include "Bang/SelectionFramebuffer.h"
#include "Bang/EditorDebugGameObject.h"
#include "Bang/EditorGizmosGameObject.h"
#include "Bang/EditorTranslateAxisGroup.h"

EditorScene::EditorScene() : Scene()
{
    m_gizmosGameObject = new EditorGizmosGameObject("BANG_GizmosGameObject");
    m_gizmosGameObject->SetParent(this);
    m_gizmosGameObject->Init();

    m_debugGameObject = new EditorDebugGameObject();
    m_debugGameObject->SetParent(this);

    m_edCameraGameObject = new EditorCamera();
    m_edCameraGameObject->SetParent(this);
    SetCamera(m_edCameraGameObject->GetCamera());

    m_floor = new EditorFloor();
    m_floor->SetParent(this);

    m_fpsCounter = new GameObject("FPS_Counter");
    m_fpsCounter->AddHideFlag(HideFlags::HideAndDontSave);
    m_fpsCounter->ChangeTransformByRectTransform();
    m_fpsCounter->SetParent(this);
    RectTransform *rtrans = Object::SCast<RectTransform>(m_fpsCounter->transform);
    m_fpsCounterText = m_fpsCounter->AddComponent<UIText>();
    rtrans->SetAnchors(Vector2(1,1), Vector2(1,1));
    rtrans->SetMarginLeft(-100);
    rtrans->SetMarginBot(-100);
    rtrans->SetMarginRight(15);
    rtrans->SetMarginTop(15);
    m_fpsCounterText->SetHorizontalAlign(UIText::HorizontalAlignment::Right);
    m_fpsCounterText->SetVerticalAlign(UIText::VerticalAlignment::Top);
    m_fpsCounterText->SetTextSize(10);

    AddHideFlag(HideFlags::HideInGame); // To enable updates in editor time
}

EditorScene::~EditorScene()
{
    Hierarchy::GetInstance()->Clear(); // To avoid potential bugs (seriously xd)
}

void EditorScene::CloneInto(ICloneable *clone) const
{
    EditorScene *edScene = Object::SCast<EditorScene>(clone);
    Scene::CloneInto(edScene);
}

void EditorScene::_OnStart()
{
    Scene::_OnStart();
}

void EditorScene::OnEditorUpdate()
{
    Scene::OnEditorUpdate();

    static List<int> latestFPS = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int fps = 1.0f / (Time::s_deltaTime + 0.0001f);

    latestFPS.PushFront(fps);
    latestFPS.PopBack();
    int fpsSum = 0;
    for (int f : latestFPS) { fpsSum += f; }
    int fpsMean = fpsSum / latestFPS.Size();
    m_fpsCounterText->SetContent("FPS: " + String(fpsMean));

    static double totalTime = 0.0;
    static int totalFrameCount = 0;
    totalTime += Time::s_deltaTime;
    ++totalFrameCount;

    // Send to back so that it renders on top of any user canvas element
    m_fpsCounter->SetParent(this);
}

void EditorScene::SetEditorCamera()
{
    SetCamera( m_edCameraGameObject->GetCamera() );
}

Camera *EditorScene::GetCamera() const
{
    return Scene::GetCamera();
}

EditorCamera *EditorScene::GetEditorCamera() const
{
    return m_edCameraGameObject;
}

EditorGizmosGameObject *EditorScene::GetGizmosGameObject() const
{
    return m_gizmosGameObject;
}

// Dont erase this override, this bypasses stuff Scene does
void EditorScene::PostRead(const XMLNode &xmlInfo)
{
    GameObject::PostRead(xmlInfo);
}

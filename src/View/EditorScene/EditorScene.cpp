#include "EditorScene.h"

#include "Time.h"
#include "List.h"
#include "Debug.h"
#include "Camera.h"
#include "UIText.h"
#include "Hierarchy.h"
#include "Transform.h"
#include "EditorState.h"
#include "EditorFloor.h"
#include "EditorCamera.h"
#include "RectTransform.h"
#include "SelectionFramebuffer.h"
#include "EditorDebugGameObject.h"
#include "EditorGizmosGameObject.h"
#include "EditorTranslateAxisGroup.h"

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

    m_fpsCounter = new GameObject();
    m_fpsCounter->AddHideFlag(HideFlags::HideAndDontSave);
    m_fpsCounter->ChangeTransformByRectTransform();
    m_fpsCounter->SetParent(this);
    RectTransform *rtrans = Object::SCast<RectTransform>(m_fpsCounter->transform);
    m_fpsCounterText = m_fpsCounter->AddComponent<UIText>();
    rtrans->SetAnchors(Vector2(-1,-1), Vector2(1,1));
    rtrans->SetMargins(15);
    m_fpsCounterText->SetHorizontalAlign(UIText::HorizontalAlignment::Right);
    m_fpsCounterText->SetVerticalAlign(UIText::VerticalAlignment::Top);
    m_fpsCounterText->SetTextSize(10);
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

ICloneable *EditorScene::Clone() const
{
    EditorScene *edScene = new EditorScene();
    CloneInto(edScene);
    return edScene;
}

void EditorScene::_OnStart()
{
    Scene::_OnStart();
}

void EditorScene::OnUpdate()
{
    Scene::OnUpdate();

    static List<int> latestFPS = {0,0,0,0,0};
    int fps = 1.0f / (Time::s_deltaTime + 0.0001f);
    latestFPS.PushFront(fps);
    latestFPS.PopBack();
    int fpsSum = 0;
    for (int f : latestFPS) { fpsSum += f; }
    int fpsMean = fpsSum / latestFPS.Size();
    m_fpsCounterText->SetContent("FPS: " + String(fpsMean));
}

void EditorScene::SetEditorCamera()
{
    SetCamera( m_edCameraGameObject->GetCamera() );
}

Camera *EditorScene::GetCamera() const
{
    if (!EditorState::IsPlaying())
    {
        return m_edCameraGameObject->GetCamera();
    }
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

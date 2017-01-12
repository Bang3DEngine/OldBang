#include "EditorScene.h"

#include "Time.h"
#include "List.h"
#include "UIText.h"
#include "Hierarchy.h"
#include "Transform.h"
#include "EditorFloor.h"
#include "EditorCamera.h"
#include "SelectionFramebuffer.h"
#include "EditorDebugGameObject.h"
#include "EditorTranslateAxisGroup.h"

EditorScene::EditorScene() : Scene()
{
    EditorGameObject *gizmosGo = new EditorGameObject("BANG_GizmosGameObject");
    gizmosGo->SetParent(this);
    Gizmos::SetGizmosGameObject(gizmosGo);

    m_debugGameObject = new EditorDebugGameObject();
    m_debugGameObject->SetParent(this);

    m_cameraGameObject = new EditorCamera();
    m_cameraGameObject->SetParent(this);
    SetCamera(m_cameraGameObject->GetCamera());

    //axises = new EditorTranslateAxisGroup();
    //axises->SetParent(this);

    m_floor = new EditorFloor();
    m_floor->SetParent(this);

    m_fpsCounter = new EditorGameObject();
    m_fpsCounter->SetParent(this);
    m_fpsCounterText = m_fpsCounter->AddComponent<UIText>();
    m_fpsCounterText->SetTextSize(10);
    m_fpsCounter->transform->SetLocalPosition(Vector3(0.7f, 0.92f, 1.0f));
}

EditorScene::~EditorScene()
{
    Gizmos::SetGizmosGameObject(nullptr);
    Hierarchy::GetInstance()->Clear(); // To avoid potential bugs (seriously xd)
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

bool EditorScene::IsEditorGameObject() const
{
    return true;
}

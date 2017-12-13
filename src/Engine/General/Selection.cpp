#include "Bang/Selection.h"

#include "Bang/Scene.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/SceneManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

GameObject *Selection::GetOveredGameObject()
{
    if (!Input::IsMouseInsideScreen()) { return nullptr; }
    return GetOveredGameObject(  SceneManager::GetActiveScene() );
}

GameObject *Selection::GetOveredGameObject(Scene *scene)
{
    if (!scene) { return nullptr; }

    Camera *cam = scene->GetCamera();
    if (cam)
    {
        Vector2i mouseViewportPoint = Input::GetMousePositionScreen();
        mouseViewportPoint = cam->FromScreenPointToViewportPoint(mouseViewportPoint);
        SelectionFramebuffer *sfb = cam->GetSelectionFramebuffer();
        if (sfb)
        {
            GameObject *selGo =
                        sfb->GetGameObjectInViewportPoint(mouseViewportPoint);
            if (selGo) { return selGo; }
        }
    }
    return nullptr;
}

#include "Bang/GameObjectFactory.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/UIMask.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIButton.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputText.h"
#include "Bang/UIScrollBar.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/UIListDriver.h"
#include "Bang/RectTransform.h"
#include "Bang/ObjectManager.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/DirectionalLight.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

GameObject *GameObjectFactory::CreateGameObject(bool addTransform)
{
    GameObject *go = ObjectManager::Create<GameObject>();
    if (addTransform) { go->AddComponent<Transform>(); }
    return go;
}

GameObject *GameObjectFactory::CreateUIGameObject(bool addComponents)
{
    GameObject *go = ObjectManager::Create<GameObject>();
    GameObjectFactory::CreateUIGameObjectInto(go, addComponents);
    return go;
}

GameObject *GameObjectFactory::CreateGameObject(const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);
    return go;
}

GameObject *GameObjectFactory::CreateUIGameObject(const String &name)
{
    GameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->SetName(name);
    return go;
}

void GameObjectFactory::CreateUIGameObjectInto(GameObject *go,  bool addComps)
{
    if (addComps)
    {
        go->AddComponent<RectTransform>();
    }
}

Scene *GameObjectFactory::CreateScene()
{
    Scene *scene = ObjectManager::Create<Scene>();
    scene->AddComponent<Transform>();
    return scene;
}

Scene *GameObjectFactory::CreateUIScene()
{
    Scene *scene = ObjectManager::Create<Scene>();
    GameObjectFactory::CreateUIGameObjectInto(scene);
    GameObjectFactory::CreateUICanvasInto(scene);
    return scene;
}

Scene *GameObjectFactory::CreateDefaultScene()
{
    Scene *scene = GameObjectFactory::CreateScene();

    GameObject *cube = GameObjectFactory::CreateGameObject();
    MeshRenderer *mr = cube->AddComponent<MeshRenderer>();
    mr->SetMesh( MeshFactory::GetCube() );

    GameObject *light = GameObjectFactory::CreateGameObject();
    DirectionalLight *dl = light->AddComponent<DirectionalLight>();
    light->GetTransform()->SetPosition( Vector3(5,4,3) );
    light->GetTransform()->LookAt( Vector3::Zero );

    GameObject *cameraGo = GameObjectFactory::CreateGameObject();
    cameraGo->GetTransform()->SetPosition( Vector3(5,4,3) );
    cameraGo->GetTransform()->LookAt( Vector3::Zero );
    Camera *cam = cameraGo->AddComponent<Camera>();
    cam->SetClearColor(Color::LightBlue);
    scene->SetCamera(cam);

    scene->SetAsChild(cube);
    scene->SetAsChild(light);
    scene->SetAsChild(cameraGo);
    return scene;
}

UICanvas *GameObjectFactory::CreateUICanvas()
{
    GameObject *go = GameObjectFactory::CreateUIGameObject();
    return GameObjectFactory::CreateUICanvasInto(go);
}

UICanvas* GameObjectFactory::CreateUICanvasInto(GameObject *go)
{
    UICanvas *canvas = go->AddComponent<UICanvas>();
    go->SetName("Canvas");
    return canvas;
}

UIImageRenderer *GameObjectFactory::CreateUIImage(const Color &color)
{
    GameObject *go = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *img = go->AddComponent<UIImageRenderer>();
    img->SetTint(color);
    go->SetName("Image");
    return img;
}

UIImageRenderer *GameObjectFactory::CreateUIImage(const Color &color,
                                                  const Vector2i &size)
{
    UIImageRenderer *img = GameObjectFactory::CreateUIImage(color);
    UILayoutElement *le = img->GetGameObject()->AddComponent<UILayoutElement>();
    le->SetMinSize(size);
    le->SetPreferredSize(size);
    return img;
}

UIListDriver *GameObjectFactory::CreateUIListInto(GameObject *go)
{
    return UIListDriver::CreateInto(go);
}
UIListDriver *GameObjectFactory::CreateUIList()
{
    return UIListDriver::CreateInto( GameObjectFactory::CreateUIGameObject("List") );
}

UIInputText *GameObjectFactory::CreateUIInputTextInto(GameObject *go)
{
    return UIInputText::CreateInto(go);
}
UIInputText *GameObjectFactory::CreateUIInputText()
{
    return GameObjectFactory::CreateUIInputTextInto(
                        GameObjectFactory::CreateUIGameObject("InputText") );
}

UIInputNumber *GameObjectFactory::CreateUIInputNumberInto(GameObject *go)
{
    return UIInputNumber::CreateInto(go);
}

UIInputNumber *GameObjectFactory::CreateUIInputNumber()
{
    return GameObjectFactory::CreateUIInputNumberInto(
                    GameObjectFactory::CreateUIGameObject("InputNumber") );
}

UIButtonDriver *GameObjectFactory::CreateUIButtonInto(GameObject *go)
{
    return UIButtonDriver::CreateInto(go);
}
UIButtonDriver* GameObjectFactory::CreateUIButton()
{
    return UIButtonDriver::CreateInto(
                GameObjectFactory::CreateUIGameObject("Button") );
}

UIButtonDriver *GameObjectFactory::CreateUIButton(const String &text,
                                                  Texture2D *icon)
{
    const Vector2i size(15);
    UIButtonDriver *btn = GameObjectFactory::CreateUIButton();

    if (!text.IsEmpty()) { btn->GetText()->SetContent(text); }

    if (icon) { btn->SetIcon(icon, size, 5); }

    return btn;
}

UILabel *GameObjectFactory::CreateUILabelInto(GameObject *go)
{
    return UILabel::CreateInto(go);
}
UILabel *GameObjectFactory::CreateUILabel()
{
    return UILabel::CreateInto( GameObjectFactory::CreateUIGameObject("Label") );
}

UIScrollArea *GameObjectFactory::CreateUIScrollArea()
{
    return UIScrollArea::CreateInto(
                        GameObjectFactory::CreateUIGameObject("ScrollArea") );
}

UIScrollBar *GameObjectFactory::CreateUIScrollBarInto(GameObject *go)
{
    return UIScrollBar::CreateInto(go);
}
UIScrollBar *GameObjectFactory::CreateUIScrollBar()
{
    return UIScrollBar::CreateInto( GameObjectFactory::CreateUIGameObject("ScrollBar") );
}

UIScrollPanel *GameObjectFactory::CreateUIScrollPanelInto(GameObject *go)
{
    return UIScrollPanel::CreateInto(go);
}
UIScrollPanel *GameObjectFactory::CreateUIScrollPanel()
{
    return GameObjectFactory::CreateUIScrollPanelInto(
                GameObjectFactory::CreateUIGameObject("ScrollPanel") );
}

UIScrollArea* GameObjectFactory::CreateUIScrollAreaInto(GameObject *go)
{
    return UIScrollArea::CreateInto(go);
}

GameObject *GameObjectFactory::CreateUISpacer(LayoutSizeType sizeType,
                                              const Vector2i &space)
{
    GameObject *spacerGo = GameObjectFactory::CreateUIGameObject("Separator");
    UILayoutElement *le = spacerGo->AddComponent<UILayoutElement>();

    le->SetMinSize( Vector2i(0) );
    le->SetPreferredSize( Vector2i(0) );
    le->SetFlexibleSize( Vector2(0) );

    if (sizeType == LayoutSizeType::Min) { le->SetMinSize(space); }
    else if (sizeType == LayoutSizeType::Preferred) { le->SetPreferredSize(space); }
    else { le->SetFlexibleSize( Vector2(space) ); }
    return spacerGo;
}
GameObject *GameObjectFactory::CreateUIHSpacer(LayoutSizeType sizeType,
                                               int spaceX)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateUISpacer(sizeType, Vector2i(spaceX, 0) );
    return spacerGo;
}
GameObject *GameObjectFactory::CreateUIVSpacer(LayoutSizeType sizeType,
                                               int spaceY)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateUISpacer(sizeType, Vector2i(0, spaceY) );
    return spacerGo;
}

GameObject *GameObjectFactory::CreateUISeparator(LayoutSizeType sizeType,
                                                 const Vector2i &space,
                                                 float linePercent)
{
    GameObject *sepGo = GameObjectFactory::CreateUISpacer(sizeType, space);
    LineRenderer *lr = sepGo->AddComponent<LineRenderer>();
    lr->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    lr->SetRenderPass(RenderPass::Canvas);

    UILayoutElement *le = sepGo->GetComponent<UILayoutElement>();
    le->SetPreferredSize( Vector2i::Max(space, Vector2i::One) );
    bool horizontal = (space.x == 0);
    if (horizontal)
    {
        le->SetFlexibleSize( Vector2(99999999, 0) );
        lr->SetPoints( {Vector3(-linePercent,0,0), Vector3(linePercent,0,0)} );
    }
    else
    {
        le->SetFlexibleSize( Vector2(0, 99999999) );
        lr->SetPoints( {Vector3(0,-linePercent,0), Vector3(0,linePercent,0)} );
    }
    return sepGo;
}

GameObject *GameObjectFactory::CreateUIHSeparator(LayoutSizeType sizeType,
                                                  int spaceY,
                                                  float linePercent)
{
    GameObject *sepGo =
            GameObjectFactory::CreateUISeparator(sizeType, Vector2i(0, spaceY),
                                                 linePercent);
    return sepGo;
}
GameObject *GameObjectFactory::CreateUIVSeparator(LayoutSizeType sizeType,
                                                  int spaceX,
                                                  float linePercent)
{
    GameObject *sepGo =
            GameObjectFactory::CreateUISeparator(sizeType, Vector2i(spaceX, 0),
                                                 linePercent);
    return sepGo;
}

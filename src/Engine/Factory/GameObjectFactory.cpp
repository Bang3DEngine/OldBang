#include "Bang/GameObjectFactory.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIButton.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputText.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/UIListDriver.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITintedButton.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/DirectionalLight.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

GameObject *GameObjectFactory::CreateGameObject(bool addTransform)
{
    GameObject *go = new GameObject();
    if (addTransform) { go->AddComponent<Transform>(); }
    return go;
}

GameObject *GameObjectFactory::CreateUIGameObject(bool addRectTransform)
{
    GameObject *go = new GameObject();
    if (addRectTransform) { go->AddComponent<RectTransform>(); }
    go->AddComponent<UIFocusTaker>();
    return go;
}

Scene *GameObjectFactory::CreateScene()
{
    Scene *scene = new Scene();
    scene->AddComponent<Transform>();
    return scene;
}

Scene *GameObjectFactory::CreateUIScene()
{
    Scene *scene = new Scene();
    scene->AddComponent<RectTransform>();
    scene->AddComponent<UIFocusTaker>();
    scene->AddComponent<UICanvas>();
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
    light->transform->SetPosition( Vector3(5,4,3) );
    light->transform->LookAt( Vector3::Zero );

    GameObject *cameraGo = GameObjectFactory::CreateGameObject();
    cameraGo->transform->SetPosition( Vector3(5,4,3) );
    cameraGo->transform->LookAt( Vector3::Zero );
    Camera *cam = cameraGo->AddComponent<Camera>();
    cam->SetClearColor(Color::LightBlue);
    scene->SetCamera(cam);

    scene->AddChild(cube);
    scene->AddChild(light);
    scene->AddChild(cameraGo);
    return scene;
}

UIListDriver *GameObjectFactory::CreateGUIList(bool vertical)
{
    GameObject *listGo = GameObjectFactory::CreateUIGameObject();

    UIListDriver *listDriver = listGo->AddComponent<UIListDriver>();
    listDriver->Create();

    return listDriver;
}

GameObject *GameObjectFactory::CreateGUIInputText()
{
    return UIInputText::CreateGameObject();
}

UIButtonDriver* GameObjectFactory::CreateGUIButton()
{
    GameObject *container = GameObjectFactory::CreateUIGameObject();
    UIFrameLayout *fl = container->AddComponent<UIFrameLayout>();
    fl->SetPaddings(5);
    fl->SetPaddingTop(5);
    fl->SetPaddingRight(10);
    fl->SetPaddingLeft (10);

    UILayoutElement *le = container->AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(-1) );
    le->SetPreferredSize( Vector2i(-1) );
    le->SetFlexibleSize( Vector2(0) );

    GameObject *bg = GameObjectFactory::CreateUIGameObject(true);
    bg->SetName("GUIButton_Background");
    UIImageRenderer *bgImg = bg->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);
    bg->SetParent(container);

    GameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->AddComponent<UIFrameLayout>();
    go->SetName("GUIButton");

    GameObject *label = GameObjectFactory::CreateGUILabel();
    label->SetName("GUIButton_Label");
    label->GetComponentInChildren<UITextRenderer>()->SetTextColor(Color::Black);
    label->SetParent(go);

    UITintedButton *bgWTint = go->AddComponent<UITintedButton>();
    bgWTint->SetMode(UIButtonMode::UseRectTransform);
    bgWTint->AddToTint(bg);
    bgWTint->SetIdleTintColor(bgImg->GetTint());
    bgWTint->SetOverTintColor( Color(Vector3(0.95), 1) );
    bgWTint->SetPressedTintColor( Color(Vector3(0.9), 1) );
    bgWTint->AddAgent(bg);

    UIButtonDriver *buttonDriv = container->AddComponent<UIButtonDriver>();
    buttonDriv->SetBackground(bgImg);
    buttonDriv->SetButton(bgWTint);
    buttonDriv->SetText(label->GetComponentInChildren<UITextRenderer>());

    go->SetParent(container);

    return buttonDriv;
}

GameObject *GameObjectFactory::CreateGUILabel(const String &content)
{
    GameObject *maskGo = GameObjectFactory::CreateUIGameObject();
    maskGo->SetName("GUILabel_Mask");
    maskGo->AddComponent<UIFrameLayout>();
    UIMask *mask = maskGo->AddComponent<UIMask>();
    maskGo->AddComponent<UIImageRenderer>(); // Quad mask

    GameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    textContainer->SetName("GUILabel_TextContainer");
    textContainer->SetParent(maskGo);
    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetTextSize(12);
    text->SetContent(content);
    text->SetWrapping(false);

    return maskGo;
}

GameObject *GameObjectFactory::CreateGUIScrollArea()
{
    return UIScrollArea::CreateGameObject();
}

GameObject *GameObjectFactory::CreateGUISpacer(LayoutSizeType sizeType,
                                               const Vector2i &space)
{
    GameObject *spacerGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *le = spacerGo->AddComponent<UILayoutElement>();

    le->SetMinSize( Vector2i(0) );
    le->SetPreferredSize( Vector2i(0) );
    le->SetFlexibleSize( Vector2(0) );

    if (sizeType == LayoutSizeType::Min) { le->SetMinSize(space); }
    else if (sizeType == LayoutSizeType::Preferred) { le->SetPreferredSize(space); }
    else { le->SetFlexibleSize( Vector2(space) ); }
    return spacerGo;
}
GameObject *GameObjectFactory::CreateGUIHSpacer(LayoutSizeType sizeType,
                                                int spaceX)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateGUISpacer(sizeType, Vector2i(spaceX, 0) );
    return spacerGo;
}
GameObject *GameObjectFactory::CreateGUIVSpacer(LayoutSizeType sizeType,
                                                int spaceY)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateGUISpacer(sizeType, Vector2i(0, spaceY) );
    return spacerGo;
}

GameObject *GameObjectFactory::CreateGUISeparator(LayoutSizeType sizeType,
                                                  const Vector2i &space)
{
    GameObject *sepGo = GameObjectFactory::CreateGUISpacer(sizeType, space);
    LineRenderer *lr = sepGo->AddComponent<LineRenderer>();
    lr->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    lr->SetRenderPass(RenderPass::Canvas);

    UILayoutElement *le = sepGo->GetComponent<UILayoutElement>();
    le->SetPreferredSize( Vector2i::Max(space, Vector2i::One) );
    bool horizontal = (space.x == 0);
    constexpr float s = 0.95f;
    if (horizontal)
    {
        le->SetFlexibleSize( Vector2(99999999, 0) );
        lr->SetPoints( {Vector3(-s,0,0), Vector3(s,0,0)} );
    }
    else
    {
        le->SetFlexibleSize( Vector2(0, 99999999) );
        lr->SetPoints( {Vector3(0,-s,0), Vector3(0,s,0)} );
    }
    return sepGo;
}

GameObject *GameObjectFactory::CreateGUIHSeparator(LayoutSizeType sizeType,
                                                   int spaceY)
{
    GameObject *sepGo =
            GameObjectFactory::CreateGUISeparator(sizeType, Vector2i(0, spaceY) );
    return sepGo;
}
GameObject *GameObjectFactory::CreateGUIVSeparator(LayoutSizeType sizeType,
                                                   int spaceX)
{
    GameObject *sepGo =
            GameObjectFactory::CreateGUISeparator(sizeType, Vector2i(spaceX, 0) );
    return sepGo;
}

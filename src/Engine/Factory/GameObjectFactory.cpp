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
#include "Bang/UIGameObject.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITintedButton.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/DirectionalLight.h"

USING_NAMESPACE_BANG

GameObject*
GameObjectFactory::CreateGameObject(const String &gameObjectClassName)
{
    if (gameObjectClassName == GameObject::GetClassNameStatic())
    {
        return GameObjectFactory::CreateGameObject(false);
    }
    return GameObjectFactory::CreateUIGameObject(false);
}

bool GameObjectFactory::ExistsGameObjectClass(const String &gameObjectClassName)
{
    return gameObjectClassName == GameObject::GetClassNameStatic() ||
           gameObjectClassName == UIGameObject::GetClassNameStatic();
}

GameObject *GameObjectFactory::CreateGameObject(bool addTransform)
{
    GameObject *go = new GameObject();
    if (addTransform) { go->AddComponent<Transform>(); }
    return go;
}

UIGameObject *GameObjectFactory::CreateUIGameObject(bool addRectTransform)
{
    UIGameObject *go = new UIGameObject();
    if (addRectTransform) { go->AddComponent<RectTransform>(); }
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
    scene->AddComponent<UICanvas>();
    return scene;
}

#include "Bang/Time.h"
Scene *GameObjectFactory::CreateDefaultScene()
{
    class Rotator : public Component
    {
    public:
        Rotator() {}
        void OnUpdate() override
        {
            gameObject->transform->Rotate(
                        Quaternion::AngleAxis(0.5f * Time::GetDeltaTime(),
                                              Vector3::Up) );
        }
    };

    Scene *scene = GameObjectFactory::CreateScene();

    GameObject *cube = GameObjectFactory::CreateGameObject();
    MeshRenderer *mr = cube->AddComponent<MeshRenderer>();
    mr->SetMesh( MeshFactory::GetCube() );
    cube->AddComponent<Rotator>();

    GameObject *light = GameObjectFactory::CreateGameObject();
    DirectionalLight *dl = light->AddComponent<DirectionalLight>();
    light->transform->SetPosition( Vector3(5,5,5) );
    light->transform->LookAt( Vector3(0, 0, 0) );

    scene->SetFirstFoundCameraOrDefaultOne();
    scene->GetCamera()->SetClearColor(Color::LightBlue);
    scene->SetFirstFoundCameraOrDefaultOne();

    scene->AddChild(cube);
    scene->AddChild(light);
    return scene;
}

UIGameObject *GameObjectFactory::CreateGUIInputText()
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

    UIGameObject *bg = GameObjectFactory::CreateUIGameObject(true);
    bg->SetName("GUIButton_Background");
    UIImageRenderer *bgImg = bg->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);
    bg->SetParent(container);

    UIGameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->AddComponent<UIFrameLayout>();
    go->SetName("GUIButton");

    UIGameObject *label = GameObjectFactory::CreateGUILabel();
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

UIGameObject *GameObjectFactory::CreateGUILabel(const String &content)
{
    UIGameObject *maskGo = GameObjectFactory::CreateUIGameObject();
    maskGo->SetName("GUILabel_Mask");
    maskGo->AddComponent<UIFrameLayout>();
    UIMask *mask = maskGo->AddComponent<UIMask>();
    maskGo->AddComponent<UIImageRenderer>(); // Quad mask

    UIGameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    textContainer->SetName("GUILabel_TextContainer");
    textContainer->SetParent(maskGo);
    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetTextSize(12);
    text->SetContent(content);
    text->SetWrapping(false);

    return maskGo;
}

UIGameObject *GameObjectFactory::CreateGUIScrollArea()
{
    return UIScrollArea::CreateGameObject();
}

UIGameObject *GameObjectFactory::CreateGUISpacer(LayoutSizeType sizeType,
                                                 const Vector2i &space)
{
    UIGameObject *spacerGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *le = spacerGo->AddComponent<UILayoutElement>();
    if (sizeType == LayoutSizeType::Min)
    {
        le->SetMinSize( Vector2i(space) );
        le->SetPreferredSize( Vector2i(0) );
        le->SetFlexibleSize( Vector2(0) );
    }
    else if (sizeType == LayoutSizeType::Preferred)
    {
        le->SetMinSize( Vector2i(0) );
        le->SetPreferredSize( Vector2i(space) );
        le->SetFlexibleSize( Vector2(0) );
    }
    else // Flexible
    {
        le->SetMinSize( Vector2i(0) );
        le->SetPreferredSize( Vector2i(0) );
        le->SetFlexibleSize( Vector2(space) );
    }
    return spacerGo;
}
UIGameObject *GameObjectFactory::CreateGUIHSpacer(LayoutSizeType sizeType,
                                                  int spaceX)
{
    UIGameObject *spacerGo =
            GameObjectFactory::CreateGUISpacer(sizeType, Vector2i(spaceX, 0) );
    spacerGo->GetComponent<UILayoutElement>()->SetPreferredHeight(0);
    return spacerGo;
}
UIGameObject *GameObjectFactory::CreateGUIVSpacer(LayoutSizeType sizeType,
                                                  int spaceY)
{
    UIGameObject *spacerGo =
            GameObjectFactory::CreateGUISpacer(sizeType, Vector2i(0, spaceY) );
    spacerGo->GetComponent<UILayoutElement>()->SetPreferredWidth(0);
    return spacerGo;
}

UIGameObject *GameObjectFactory::CreateGUISeparator(LayoutSizeType sizeType,
                                                    const Vector2i &space)
{
    UIGameObject *sepGo = GameObjectFactory::CreateGUISpacer(sizeType, space);
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

UIGameObject *GameObjectFactory::CreateGUIHSeparator(LayoutSizeType sizeType,
                                                     int spaceY)
{
    UIGameObject *sepGo =
            GameObjectFactory::CreateGUISeparator(sizeType, Vector2i(0, spaceY) );
    return sepGo;
}
UIGameObject *GameObjectFactory::CreateGUIVSeparator(LayoutSizeType sizeType,
                                                     int spaceX)
{
    UIGameObject *sepGo =
            GameObjectFactory::CreateGUISeparator(sizeType, Vector2i(spaceX, 0) );
    return sepGo;
}

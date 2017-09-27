#include "Bang/GameObjectFactory.h"

#include "Bang/Scene.h"
#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIButton.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputText.h"
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
        le->SetPreferredSize( Vector2i(1) );
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
    bool horizontal = (space.x == 0);
    if (horizontal)
    {
        le->SetPreferredWidth(1);
        le->SetFlexibleWidth(99999999);
        lr->SetPoints( {Vector3(-1,0,0), Vector3(1,0,0)} );
    }
    else
    {
        le->SetPreferredHeight(1);
        le->SetFlexibleHeight(9999999);
        lr->SetPoints( {Vector3(0,-1,0), Vector3(0,1,0)} );
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

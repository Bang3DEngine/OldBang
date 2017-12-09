#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class UIList;
FORWARD class UITree;
FORWARD class UILabel;
FORWARD class UIButton;
FORWARD class UICanvas;
FORWARD class Texture2D;
FORWARD class GameObject;
FORWARD class UIInputText;
FORWARD class UIScrollBar;
FORWARD class UIScrollArea;
FORWARD class UIScrollPanel;
FORWARD class UIInputNumber;
FORWARD class UIImageRenderer;

class GameObjectFactory
{
public:

    static GameObject* CreateGameObject(bool addTransform = true);
    static GameObject* CreateUIGameObject(bool addComponents = true);
    static GameObject* CreateGameObjectNamed(const String &name);
    static GameObject* CreateUIGameObjectNamed(const String &name);
    static void CreateUIGameObjectInto(GameObject *go, bool addComponents = true);

    // Disallow implicit conversions on above functions
    template <class T> GameObject* CreateGameObject(T) = delete;
    template <class T> GameObject* CreateUIGameObject(T) = delete;

    static Scene* CreateScene(bool addTransform = true);
    static Scene* CreateUIScene();
    static Scene* CreateDefaultSceneInto(Scene *scene);

    static UICanvas*        CreateUICanvas();
    static UICanvas*        CreateUICanvasInto(GameObject *go);
    static UIImageRenderer* CreateUIImage(const Color &color = Color::White);
    static UIImageRenderer* CreateUIImage(const Color &color,
                                          const Vector2i &size);
    static UIList*          CreateUIListInto(GameObject *go);
    static UIList*          CreateUIList();
    static UITree*          CreateUITreeInto(GameObject *go);
    static UITree*          CreateUITree();
    static UIInputText*     CreateUIInputTextInto(GameObject *go);
    static UIInputText*     CreateUIInputText();
    static UIInputNumber*   CreateUIInputNumberInto(GameObject *go);
    static UIInputNumber*   CreateUIInputNumber();
    static UIButton*  CreateUIButtonInto(GameObject *go);
    static UIButton*  CreateUIButton();
    static UIButton*  CreateUIButton(const String &text,
                                           Texture2D *icon = nullptr);
    static UILabel*         CreateUILabel();
    static UILabel*         CreateUILabelInto(GameObject *go);
    static UIScrollArea*    CreateUIScrollAreaInto(GameObject *go);
    static UIScrollArea*    CreateUIScrollArea();
    static UIScrollBar*     CreateUIScrollBarInto(GameObject *go);
    static UIScrollBar*     CreateUIScrollBar();
    static UIScrollPanel*   CreateUIScrollPanelInto(GameObject *go);
    static UIScrollPanel*   CreateUIScrollPanel();

    static GameObject* CreateUIHSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceY = 1,
                            float linePercent = 1.0f);
    static GameObject* CreateUIVSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceX = 1,
                            float linePercent = 1.0f);

    static GameObject* CreateUISpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            const Vector2i &space = Vector2i(1));
    static GameObject* CreateUIHSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceX = 1);
    static GameObject* CreateUIVSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceY = 1);

    GameObjectFactory() = delete;

private:
    static GameObject* CreateUISeparator(LayoutSizeType sizeType,
                                         const Vector2i &space,
                                         float linePercent);

};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H

#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class UILabel;
FORWARD class UICanvas;
FORWARD class GameObject;
FORWARD class UIInputText;
FORWARD class UIScrollBar;
FORWARD class UIListDriver;
FORWARD class UIScrollArea;
FORWARD class UIScrollPanel;
FORWARD class UIButtonDriver;
FORWARD class UIImageRenderer;

class GameObjectFactory
{
public:
    static GameObject* CreateGameObject(bool addTransform = true);
    static GameObject* CreateUIGameObject(bool addComponents = true);
    static void CreateUIGameObjectInto(GameObject *go, bool addComponents = true);

    static Scene* CreateScene();
    static Scene* CreateUIScene();
    static Scene* CreateDefaultScene();

    static UICanvas*        CreateUICanvas();
    static UICanvas*        CreateUICanvasInto(GameObject *go);
    static UIImageRenderer* CreateUIImage(const Color &color = Color::White);
    static UIImageRenderer* CreateUIImage(const Color &color,
                                          const Vector2i &size);
    static UIListDriver*    CreateUIListInto(GameObject *go);
    static UIListDriver*    CreateUIList();
    static UIInputText*     CreateUIInputTextInto(GameObject *go);
    static UIInputText*     CreateUIInputText();
    static UIButtonDriver*  CreateUIButtonInto(GameObject *go);
    static UIButtonDriver*  CreateUIButton();
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
                            int spaceY = 1);
    static GameObject* CreateUIVSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceX = 1);

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
                                          const Vector2i &space);

};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H

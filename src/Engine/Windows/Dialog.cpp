#include "Bang/Dialog.h"

#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/UIButton.h"
#include "Bang/Transform.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

DialogWindow *Bang::Dialog::Error(const String &title,
                                  const String &msg)
{
    DialogWindow *dialog = nullptr;

    Window *topWindow = Application::GetTopWindow();
    if (topWindow)
    {
        dialog = Application::GetInstance()->CreateDialogWindow(topWindow);
        dialog->SetTitle(title);
        dialog->SetSize(300, 150);

        Scene *scene = new Scene();
        scene->AddComponent<Transform>();

        UIGameObject *container = GameObjectFactory::CreateUIGameObject();
        UIFrameLayout *containerL = container->AddComponent<UIFrameLayout>();
        containerL->SetPaddings(10);

        UIGameObject *mainVLayoutGo = GameObjectFactory::CreateUIGameObject();
        UIVerticalLayout *mainHLayout = mainVLayoutGo->AddComponent<UIVerticalLayout>();

        UIGameObject *iconGo = GameObjectFactory::CreateUIGameObject();
        UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
        iconLE->SetMinSize( Vector2i(50) );
        UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
        icon->SetTint(Color::Red);
        iconGo->GetRectTransform()->SetAnchors( Vector2::Zero );
        iconGo->GetRectTransform()->SetMargins(-25);

        UIGameObject *hLayoutGo = GameObjectFactory::CreateUIGameObject();
        UIHorizontalLayout *hLayout = hLayoutGo->AddComponent<UIHorizontalLayout>();
        UILayoutElement *hLayoutLE = hLayoutGo->AddComponent<UILayoutElement>();
        hLayoutLE->SetPreferredSize( Vector2i::One );
        hLayoutLE->SetFlexibleSize( Vector2(999.9f) );

        UIGameObject *msgGo = GameObjectFactory::CreateUIGameObject();
        UITextRenderer *text = msgGo->AddComponent<UITextRenderer>();
        text->SetContent(msg);
        text->SetTextColor(Color::White);
        text->SetTextSize(12);
        text->SetHorizontalAlign(HorizontalAlignment::Center);
        text->SetVerticalAlign(VerticalAlignment::Center);
        UILayoutElement *textLE = msgGo->AddComponent<UILayoutElement>();
        textLE->SetPreferredSize( Vector2i::One );
        textLE->SetFlexibleSize( Vector2(999.9f) );

        UIGameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
        UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
        UILayoutElement *buttonsGoLE = buttonsGo->AddComponent<UILayoutElement>();
        buttonsGoLE->SetPreferredSize( Vector2i(1, 50) );

        UIButton *button0 = GameObjectFactory::CreateGUIButton();
        button0->GetText()->SetContent("Cancel");
        UIButton *button1 = GameObjectFactory::CreateGUIButton();
        button1->GetText()->SetContent("OK");

        container->SetParent(scene);
        mainVLayoutGo->SetParent(container);
        hLayoutGo->SetParent(mainVLayoutGo);
        iconGo->SetParent(hLayoutGo);
        msgGo->SetParent(hLayoutGo);
        buttonsGo->SetParent(mainVLayoutGo);
        button0->gameObject->SetParent(buttonsGo);
        button1->gameObject->SetParent(buttonsGo);

        SceneManager::LoadScene(scene);
    }
    return dialog;
}

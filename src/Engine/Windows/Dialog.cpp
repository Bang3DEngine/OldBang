#include "Bang/Dialog.h"

#include "Bang/Paths.h"
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
#include "Bang/UITintedInteractive.h"

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

        Scene *scene = CreateMsgScene(msg);
        SceneManager::LoadScene(scene);
    }
    return dialog;
}

Path Dialog::GetFile(const String &title)
{
    DialogWindow *dialog = nullptr;
    Window *topWindow = Application::GetTopWindow();
    if (topWindow)
    {
        dialog = Application::GetInstance()->CreateDialogWindow(topWindow);
        dialog->SetTitle(title);
        dialog->SetSize(500, 500);

        Scene *scene = CreateOpenFileScene(title);
        SceneManager::LoadScene(scene);

        Application::GetInstance()->BlockingWait(dialog);
    }
    return Path::Empty;
}

Scene *Dialog::CreateOpenFileScene(const String &title)
{
    Scene *scene= new Scene();
    scene->AddComponent<Transform>();
    return scene;
}

Scene *Dialog::CreateMsgScene(const String &msg)
{
    Scene *scene = new Scene();
    scene->AddComponent<Transform>();

    UIGameObject *container = GameObjectFactory::CreateUIGameObject();
    UIFrameLayout *containerL = container->AddComponent<UIFrameLayout>();
    containerL->SetPaddings(20);

    UIGameObject *mainVLayoutGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *mainVLayout = mainVLayoutGo->AddComponent<UIVerticalLayout>();

    UIGameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetMinSize( Vector2i(45) );
    UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
    icon->SetImageTexture( EDPATH("Icons/Error.png"));
    icon->GetImageTexture()->SetFilterMode( GL::FilterMode::Linear );

    UIGameObject *hLayoutGo = GameObjectFactory::CreateUIGameObject();
    hLayoutGo->SetName("HL");
    UIHorizontalLayout *hLayout = hLayoutGo->AddComponent<UIHorizontalLayout>();
    hLayout->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);
    hLayout->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    UILayoutElement *hLayoutLE = hLayoutGo->AddComponent<UILayoutElement>();
    hLayoutLE->SetPreferredSize( Vector2i::One );
    hLayoutLE->SetFlexibleSize( Vector2(999.9f) );

    UIGameObject *msgGo = GameObjectFactory::CreateUIGameObject();
    msgGo->SetName("MSG");
    UITextRenderer *text = msgGo->AddComponent<UITextRenderer>();
    text->SetWrapping(true);
    text->SetContent(msg);
    text->SetTextColor(Color::White);
    text->SetTextSize(12);
    text->SetHorizontalAlign(HorizontalAlignment::Center);
    text->SetVerticalAlign(VerticalAlignment::Center);
    UILayoutElement *textLE = msgGo->AddComponent<UILayoutElement>();
    textLE->SetPreferredSize( Vector2i::One );
    textLE->SetFlexibleSize( Vector2(99999.9f) );

    UIGameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    UIGameObject *hSpacer = GameObjectFactory::CreateGUIHSpacer();
    UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetSpacing(20);
    buttonsHL->SetPaddings(5);

    UIButton *button0 = GameObjectFactory::CreateGUIButton();
    button0->GetText()->SetContent("Cancel");
    button0->GetInteractive()->AddClickedCallback(OnButtonClicked);

    UIButton *button1 = GameObjectFactory::CreateGUIButton();
    button1->GetText()->SetContent("OK");
    button1->GetInteractive()->AddClickedCallback(OnButtonClicked);

    scene->AddChild(container);
     container->AddChild(mainVLayoutGo);
      mainVLayoutGo->AddChild(hLayoutGo);
       hLayoutGo->AddChild(iconGo);
       hLayoutGo->AddChild(
          GameObjectFactory::CreateGUIHSpacer(LayoutSizeType::Min, 20));
       hLayoutGo->AddChild(msgGo);
      mainVLayoutGo->AddChild(
         GameObjectFactory::CreateGUIHSeparator(LayoutSizeType::Min, 20));
      mainVLayoutGo->AddChild(buttonsGo);
       buttonsGo->AddChild(hSpacer);
       buttonsGo->AddChild(button0->gameObject);
       buttonsGo->AddChild(button1->gameObject);

    return scene;
}

void Dialog::OnButtonClicked(UIInteractive *interactive)
{
    Window::Destroy( Window::GetCurrent() );
}

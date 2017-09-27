#include "Bang/Dialog.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Transform.h"
#include "Bang/UIInputText.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UITintedButton.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

Path Dialog::s_resultPath = Path::Empty;

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

Path Dialog::GetFilePath(const String &title)
{
    DialogWindow *dialog = nullptr;
    Window *topWindow = Application::GetTopWindow();
    if (topWindow)
    {
        dialog = Application::GetInstance()->CreateDialogWindow(topWindow);
        dialog->SetTitle(title);
        dialog->SetSize(300, 100);

        Scene *scene = CreateGetFilePathScene(title);
        SceneManager::LoadScene(scene);

        Dialog::s_resultPath = Path::Empty;
        Application::GetInstance()->BlockingWait(dialog);
    }
    return Dialog::s_resultPath;
}

Scene *Dialog::CreateGetFilePathScene(const String &title)
{
    Scene *scene = GameObjectFactory::CreateUIScene();
    UIFrameLayout *fl = scene->AddComponent<UIFrameLayout>();
    fl->SetPaddings(10);

    UIGameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();

    UIGameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();

    UIGameObject *inputTextGo = GameObjectFactory::CreateGUIInputText();
    UIInputText *inputText = inputTextGo->GetComponent<UIInputText>();
    inputText->GetText()->SetContent("BANGERINO PIZZERINO JODER FUNCIONA MOTHAFUCKER MECAGONTO LJAKKDSJAKSDA");
    UILayoutElement *itle = inputTextGo->AddComponent<UILayoutElement>();
    itle->SetPreferredSize( Vector2i(1, 25) );
    itle->SetFlexibleSize( Vector2(999999, 0) );

    UIButtonDriver *openButton = GameObjectFactory::CreateGUIButton();
    openButton->GetText()->SetContent("Open");
    openButton->GetButton()->AddClickedCallback(
        [inputText](UIButton *_)
        {
            Dialog::s_resultPath = Path(inputText->GetText()->GetContent());
            Window::Destroy(Window::GetCurrent());
        }
    );

    scene->AddChild(vlGo);
    vlGo->AddChild(inputTextGo);
    vlGo->AddChild(GameObjectFactory::CreateGUIVSpacer(LayoutSizeType::Min, 10));
    vlGo->AddChild(hlGo);
    hlGo->AddChild(GameObjectFactory::CreateGUIHSpacer());
    hlGo->AddChild(openButton ->gameObject);

    return scene;
}

Scene *Dialog::CreateMsgScene(const String &msg)
{
    Scene *scene = GameObjectFactory::CreateUIScene();

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

    UIButtonDriver *button0 = GameObjectFactory::CreateGUIButton();
    button0->GetText()->SetContent("Cancel");
    button0->GetButton()->AddClickedCallback(OnButtonClicked);

    UIButtonDriver *button1 = GameObjectFactory::CreateGUIButton();
    button1->GetText()->SetContent("OK");
    button1->GetButton()->AddClickedCallback(OnButtonClicked);

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

void Dialog::OnButtonClicked(UIButton *button)
{
    Window::Destroy( Window::GetCurrent() );
}

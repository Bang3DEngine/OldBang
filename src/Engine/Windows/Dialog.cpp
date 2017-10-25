#include "Bang/Dialog.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIFileList.h"
#include "Bang/UIInputText.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/UIListDriver.h"
#include "Bang/RectTransform.h"
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
        Application::GetInstance()->BlockingWait(dialog);
    }
    return dialog;
}

Path Dialog::GetFilePath(const String &title,
                         const List<String> &extensions)
{
    DialogWindow *dialog = nullptr;
    Window *topWindow = Application::GetTopWindow();
    if (topWindow)
    {
        dialog = Application::GetInstance()->CreateDialogWindow(topWindow);
        dialog->SetTitle(title);
        dialog->SetSize(500, 400);

        Scene *scene = CreateGetFilePathScene(title, extensions);
        SceneManager::LoadScene(scene);

        Dialog::s_resultPath = Path::Empty;
        Application::GetInstance()->BlockingWait(dialog);
    }
    return Dialog::s_resultPath;
}

Scene *Dialog::CreateGetFilePathScene(const String &title,
                                      const List<String> &extensions)
{
    Scene *scene = GameObjectFactory::CreateUIScene();

    UIVerticalLayout *vlMain = scene->AddComponent<UIVerticalLayout>();
    vlMain->SetPaddings(10);

    GameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::Full);
    UILayoutElement *vlLE = vlGo->AddComponent<UILayoutElement>();
    vlLE->SetFlexibleHeight(1);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Right);
    hl->SetChildrenVerticalStretch(Stretch::None);
    UILayoutElement *hlLE = hlGo->AddComponent<UILayoutElement>();
    hlLE->SetFlexibleHeight(0);

    UIListDriver *list = GameObjectFactory::CreateUIList();
    UILayoutElement *listLE = list->gameObject->AddComponent<UILayoutElement>();
    listLE->SetFlexibleSize( Vector2(1) );

    UIFileList *fileList = list->gameObject->AddComponent<UIFileList>();
    fileList->SetFileExtensions(extensions);
    fileList->SetCurrentPath(Paths::EngineAssets());

    UIInputText *inputPathText = GameObjectFactory::CreateUIInputText();
    inputPathText->GetText()->SetContent("");
    inputPathText->GetText()->SetTextSize(12);
    fileList->SetPathChangedCallback(
        [inputPathText](const Path &newPath)
        {
            inputPathText->GetText()->SetContent(newPath.GetAbsolute());
        }
    );
    fileList->SetFileAcceptedCallback( Dialog::FileAcceptedCallback );

    UILayoutElement *itLE = inputPathText->gameObject->AddComponent<UILayoutElement>();
    itLE->SetFlexibleSize(Vector2(1));

    UIButtonDriver *goButton = GameObjectFactory::CreateUIButton();
    goButton->GetText()->SetContent("Go");
    goButton->GetButton()->AddClickedCallback(
        [inputPathText, fileList](UIButton *_)
        {
            Path inputPath(inputPathText->GetText()->GetContent());
            if (inputPath.IsFile()) { inputPath = inputPath.GetDirectory(); }
            if (inputPath.IsDir())
            {
                fileList->SetCurrentPath(inputPath);
            }
        }
    );

    UIButtonDriver *openButton = GameObjectFactory::CreateUIButton();
    openButton->GetText()->SetContent("Open");
    openButton->GetButton()->AddClickedCallback(
        [fileList](UIButton *_)
        {
            Path path = fileList->GetCurrentPath();
            FileAcceptedCallback(path);
        }
    );

    GameObject *inputPathCont = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *inputPathHL = inputPathCont->AddComponent<UIHorizontalLayout>();
    inputPathHL->SetChildrenVerticalStretch(Stretch::None);
    inputPathHL->SetSpacing(10);

    UILayoutElement *inputHLLE = inputPathCont->AddComponent<UILayoutElement>();
    inputHLLE->SetFlexibleHeight(0);

    scene->AddChild(vlGo);

    vlGo->AddChild(inputPathCont);
    inputPathCont->AddChild(inputPathText->gameObject);
    inputPathCont->AddChild(goButton->gameObject);

    vlGo->AddChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 10));
    vlGo->AddChild(list->gameObject);
    vlGo->AddChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 10));

    vlGo->AddChild(hlGo);
    hlGo->AddChild(GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible));
    hlGo->AddChild(openButton->gameObject);

    scene->SetFirstFoundCameraOrDefaultOne();

    return scene;
}

Scene *Dialog::CreateMsgScene(const String &msg)
{
    Scene *scene = GameObjectFactory::CreateUIScene();

    GameObject *container = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *containerL = container->AddComponent<UIVerticalLayout>();
    containerL->SetPaddings(20);

    GameObject *mainVLayoutGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *mainVLayout = mainVLayoutGo->AddComponent<UIVerticalLayout>();

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetMinSize( Vector2i(45) );
    UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
    icon->SetImageTexture( EDPATH("Icons/Error.png"));
    icon->GetImageTexture()->SetFilterMode( GL::FilterMode::Linear );

    GameObject *hLayoutGo = GameObjectFactory::CreateUIGameObject();
    hLayoutGo->SetName("HL");
    UIHorizontalLayout *hLayout = hLayoutGo->AddComponent<UIHorizontalLayout>();
    hLayout->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);
    hLayout->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    UILayoutElement *hLayoutLE = hLayoutGo->AddComponent<UILayoutElement>();
    hLayoutLE->SetPreferredSize( Vector2i::One );

    GameObject *msgGo = GameObjectFactory::CreateUIGameObject();
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

    GameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    GameObject *hSpacer = GameObjectFactory::CreateUIHSpacer();
    UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetSpacing(20);
    buttonsHL->SetPaddings(5);

    UIButtonDriver *button0 = GameObjectFactory::CreateUIButton();
    button0->GetText()->SetContent("Cancel");
    button0->GetButton()->AddClickedCallback(OnButtonClicked);

    UIButtonDriver *button1 = GameObjectFactory::CreateUIButton();
    button1->GetText()->SetContent("OK");
    button1->GetButton()->AddClickedCallback(OnButtonClicked);

    scene->AddChild(container);
     container->AddChild(mainVLayoutGo);
      mainVLayoutGo->AddChild(hLayoutGo);
       hLayoutGo->AddChild(iconGo);
       hLayoutGo->AddChild(
          GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Min, 20));
       hLayoutGo->AddChild(msgGo);
      mainVLayoutGo->AddChild(
         GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 20));
      mainVLayoutGo->AddChild(buttonsGo);
       buttonsGo->AddChild(hSpacer);
       buttonsGo->AddChild(button0->gameObject);
       buttonsGo->AddChild(button1->gameObject);

    return scene;
}

void Dialog::FileAcceptedCallback(const Path &path)
{
    Dialog::s_resultPath = path;
    Window::Destroy(Window::GetCurrent());
}

void Dialog::OnButtonClicked(UIButton *button)
{
    Window::Destroy( Window::GetCurrent() );
}

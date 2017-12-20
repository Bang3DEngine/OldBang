#include "Bang/Dialog.h"

#include "Bang/File.h"
#include "Bang/Input.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/UIList.h"
#include "Bang/Window.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/Transform.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/UIFileList.h"
#include "Bang/UIInputText.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/UIAutoFocuser.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

bool Dialog::s_okPressed = false;
String Dialog::s_resultString = "";
Path Dialog::s_resultPath = Path::Empty;
DialogWindow* Dialog::s_currentDialog = nullptr;

DialogWindow *Bang::Dialog::Error(const String &title,
                                  const String &msg)
{
    DialogWindow *dialog = BeginCreateDialog("Error: " + title);
    dialog->SetSize(300, 150);

    Scene *scene = CreateMsgScene(msg);
    SceneManager::LoadScene(scene);

    EndCreateDialog(dialog);
    return dialog;
}

String Dialog::GetString(const String &title,
                         const String &msg,
                         const String &hint)
{
    DialogWindow *dialog = BeginCreateDialog(title);
    dialog->SetSize(300, 100);

    Scene *scene = CreateGetStringScene(msg, hint);
    SceneManager::LoadScene(scene);

    EndCreateDialog(dialog);
    return Dialog::s_okPressed ? Dialog::s_resultString : "";
}

Path Dialog::OpenFilePath(const String &title, const List<String> &extensions,
                          const Path &initialDirPath)
{
    DialogWindow *dialog = BeginCreateDialog(title);
    dialog->SetSize(500, 400);

    Scene *scene = GameObjectFactory::CreateScene(false);
    CreateOpenFilePathSceneInto(scene, false, extensions, initialDirPath);
    SceneManager::LoadScene(scene);

    EndCreateDialog(dialog);
    return Dialog::s_okPressed ? Dialog::s_resultPath : Path::Empty;
}

Path Dialog::OpenDirectory(const String &title,
                           const Path &initialDirPath)
{
    DialogWindow *dialog = BeginCreateDialog(title);
    dialog->SetSize(500, 400);

    Scene *scene = GameObjectFactory::CreateScene(false);
    CreateOpenFilePathSceneInto(scene, true, {}, initialDirPath);
    SceneManager::LoadScene(scene);

    EndCreateDialog(dialog);
    return Dialog::s_okPressed ? Dialog::s_resultPath : Path::Empty;
}

Path Dialog::SaveFilePath(const String &title, const String &extension,
                          const Path &initialDirPath)
{
    DialogWindow *dialog = BeginCreateDialog(title);
    dialog->SetSize(500, 400);

    Scene *scene = GameObjectFactory::CreateScene(false);
    CreateSaveFilePathSceneInto(scene, extension, initialDirPath);
    SceneManager::LoadScene(scene);

    EndCreateDialog(dialog);
    return Dialog::s_okPressed ? Dialog::s_resultPath : Path::Empty;
}

DialogWindow* Dialog::BeginCreateDialog(const String &title)
{
    DialogWindow *dialogWindow = nullptr;
    Window *topWindow = Application::GetTopWindow();
    if (topWindow)
    {
        dialogWindow = Application::GetInstance()->CreateDialogWindow(topWindow);
        dialogWindow->SetSize(500, 400);
        dialogWindow->SetTitle(title);
        dialogWindow->MakeCurrent();

        Dialog::s_okPressed = false;
        Dialog::s_resultString = "";
        Dialog::s_resultPath = Path::Empty;
        Dialog::s_currentDialog = dialogWindow;
    }
    return dialogWindow;
}

void Dialog::EndCreateDialog(DialogWindow *dialogWindow)
{
    Application::GetInstance()->BlockingWait(dialogWindow);
}

void Dialog::EndCurrentDialog()
{
    if (Dialog::s_currentDialog)
    {
        Window::Destroy(Dialog::s_currentDialog);
        Dialog::s_currentDialog = nullptr;
    }
}

void Dialog::CreateSaveFilePathSceneInto(Scene *scene,
                                         const String &extension,
                                         const Path &initialDirPath)
{
    UIFileList *fileList;
    UIButton *cancelButton, *botLeftButton;
    UIInputText *botInputText;

    Dialog::CreateFilePathBaseInto(scene,
                                   initialDirPath,
                                   &fileList,
                                   &cancelButton,
                                   &botLeftButton,
                                   &botInputText);

    fileList->SetFileExtensions( {extension} );

    UIButton *saveButton = botLeftButton;
    saveButton->GetText()->SetContent("Save");
    saveButton->GetButton()->AddClickedCallback(
        [fileList, botInputText](IFocusable *)
        {
            Path path = fileList->GetCurrentPath()
                        .Append(botInputText->GetText()->GetContent())
                        .AppendExtension(Extensions::GetSceneExtension());
            AcceptDialogFile(path);
        }
    );

    class SaveSceneController : public Component
    {
    public:
        void Init(UIInputText *inputText, UIFileList *fileList)
        {
            p_fileList = fileList;
            p_inputText = inputText;
        }

        void OnUpdate() override
        {
            Component::OnUpdate();

            Path currPath = p_fileList->GetCurrentSelectedPath();
            if (currPath.IsFile())
            {
                String currContent = currPath.GetNameExt();
                if (currContent != m_prevContent)
                {
                    p_inputText->GetText()->SetContent(currContent);
                    m_prevContent = currContent;
                }
            }
        }

        UIFileList *p_fileList;
        UIInputText *p_inputText;
        String m_prevContent = "";
    };

    SaveSceneController *ssc = scene->AddComponent<SaveSceneController>();
    ssc->Init(botInputText, fileList);
}

void Dialog::CreateOpenFilePathSceneInto(Scene *scene,
                                         bool openDir,
                                         const List<String> &extensions,
                                         const Path &initialDirPath)
{
    UIFileList *fileList;
    UIButton *cancelButton, *botRightButton;
    UIInputText *botInputText;

    Dialog::CreateFilePathBaseInto(scene,
                                   initialDirPath,
                                   &fileList,
                                   &cancelButton,
                                   &botRightButton,
                                   &botInputText);

    fileList->SetFileExtensions( extensions );
    fileList->SetShowOnlyDirectories(openDir);

    UIButton *openButton = botRightButton;
    openButton->GetText()->SetContent("Open");
    openButton->GetButton()->AddClickedCallback(
        [fileList](IFocusable *)
        {
            Path path = fileList->GetCurrentSelectedPath();
            AcceptDialogFile(path);
        }
    );

    botInputText->GetGameObject()->SetEnabled(false);

    class OpenFileSceneController : public Component
    {
    public:
        void Init(UIFileList *fileList, UIButton *openButton, bool openDir)
        {
            p_fileList = fileList;
            p_openButton = openButton;
            m_openDir = openDir;
        }

        void OnUpdate() override
        {
            Component::OnUpdate();

            Path currPath = m_openDir ? p_fileList->GetCurrentPath() :
                                        p_fileList->GetCurrentSelectedPath();
            bool buttonBlocked = m_openDir ? !currPath.IsDir() : !currPath.IsFile();
            p_openButton->SetBlocked(buttonBlocked);

            if (m_openDir)
            {
                p_openButton->GetText()->SetContent("Open " + currPath);
            }
        }

        bool m_openDir;
        UIFileList *p_fileList;
        UIButton *p_openButton;
        String m_prevContent = "";
    };

    OpenFileSceneController *ofsc = scene->AddComponent<OpenFileSceneController>();
    ofsc->Init(fileList, openButton, openDir);
}

void Dialog::CreateFilePathBaseInto(Scene *scene,
                                    const Path &initialDirPath,
                                    UIFileList **outFileList,
                                    UIButton **outBotLeftButton,
                                    UIButton **outBotRightButton,
                                    UIInputText **outBotInputText)
{
    GameObjectFactory::CreateUISceneInto(scene);

    UIVerticalLayout *vlMain = scene->AddComponent<UIVerticalLayout>();
    vlMain->SetPaddings(10);

    GameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::Full);
    UILayoutElement *vlLE = vlGo->AddComponent<UILayoutElement>();
    vlLE->SetFlexibleHeight(1);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = botHLGo->AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Right);
    hl->SetChildrenVerticalStretch(Stretch::None);
    UILayoutElement *hlLE = botHLGo->AddComponent<UILayoutElement>();
    hlLE->SetFlexibleHeight(0);

    UIList *list = GameObjectFactory::CreateUIList();
    UILayoutElement *listLE = list->GetGameObject()->AddComponent<UILayoutElement>();
    listLE->SetFlexibleSize( Vector2(1) );

    UIFileList *fileList = list->GetGameObject()->AddComponent<UIFileList>();

    UIInputText *inputPathText = GameObjectFactory::CreateUIInputText();
    inputPathText->GetText()->SetContent("");
    inputPathText->GetText()->SetTextSize(12);
    fileList->SetPathChangedCallback(
        [inputPathText](const Path &newPath)
        {
            inputPathText->GetText()->SetContent(newPath.GetAbsolute());
        }
    );
    fileList->SetFileAcceptedCallback( Dialog::AcceptDialogFile );

    UILayoutElement *itLE = inputPathText->GetGameObject()->
                            AddComponent<UILayoutElement>();
    itLE->SetFlexibleSize(Vector2(1));

    GameObject *inputPathCont = GameObjectFactory::CreateUIGameObject();

    UILayoutElement *inputHLLE = inputPathCont->AddComponent<UILayoutElement>();
    inputHLLE->SetFlexibleHeight(0);

    UIHorizontalLayout *inputPathHL = inputPathCont->AddComponent<UIHorizontalLayout>();
    inputPathHL->SetChildrenVerticalStretch(Stretch::None);
    inputPathHL->SetChildrenHorizontalStretch(Stretch::Full);
    inputPathHL->SetSpacing(10);

    list->GetScrollPanel()->SetHorizontalScrollBarSide(VerticalSide::Bot);
    list->GetScrollPanel()->SetVerticalScrollBarSide(HorizontalSide::Right);
    list->GetScrollPanel()->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    list->GetScrollPanel()->SetHorizontalShowScrollMode(ShowScrollMode::WhenNeeded);

    UIButton *goButton = GameObjectFactory::CreateUIButton();
    goButton->GetText()->SetContent("Go");
    goButton->GetButton()->AddClickedCallback(
        [inputPathText, fileList](IFocusable *)
        {
            Path inputPath(inputPathText->GetText()->GetContent());
            if (inputPath.IsFile()) { inputPath = inputPath.GetDirectory(); }
            if (inputPath.IsDir())
            {
                fileList->SetCurrentPath(inputPath);
            }
        }
    );

    UIButton *openButton = GameObjectFactory::CreateUIButton();
    openButton->GetGameObject()->SetName("OpenButton");
    openButton->GetText()->SetContent("Open");
    openButton->GetGameObject()->GetComponent<UILayoutElement>()->
                SetFlexibleHeight(1.0f);

    UIButton *cancelButton = GameObjectFactory::CreateUIButton();
    cancelButton->GetGameObject()->SetName("CancelButton");
    cancelButton->GetText()->SetContent("Cancel");
    cancelButton->GetGameObject()->GetComponent<UILayoutElement>()->
                  SetFlexibleHeight(1.0f);
    cancelButton->GetButton()->AddClickedCallback(
        [scene](IFocusable *)
        {
            Dialog::EndCurrentDialog();
        }
    );

    UIInputText *botInputText = GameObjectFactory::CreateUIInputText();
    botInputText->GetText()->SetContent("Unnamed");

    scene->SetAsChild(vlGo);

    vlGo->SetAsChild(inputPathCont);
    inputPathCont->SetAsChild(inputPathText->GetGameObject());
    inputPathCont->SetAsChild(goButton->GetGameObject());

    vlGo->SetAsChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 10));
    vlGo->SetAsChild(list->GetGameObject());
    vlGo->SetAsChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 10));

    vlGo->SetAsChild(botHLGo);
    botHLGo->SetAsChild(GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible,
                                                           0.0001f));
    botInputText->GetGameObject()->SetParent(botHLGo);
    botHLGo->SetName("WOW");
    botHLGo->SetAsChild(GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Preferred, 10));
    botHLGo->SetAsChild(cancelButton->GetGameObject());
    botHLGo->SetAsChild(GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Preferred, 5));
    botHLGo->SetAsChild(openButton->GetGameObject());

    scene->SetFirstFoundCamera();

    Path initDirPath = initialDirPath;
    if (!initDirPath.IsDir()) { initDirPath = Paths::Engine(); }
    fileList->SetCurrentPath(initDirPath);

    // Params
    if (outFileList)       { *outFileList       = fileList;     }
    if (outBotLeftButton)  { *outBotLeftButton  = cancelButton; }
    if (outBotRightButton) { *outBotRightButton = openButton;   }
    if (outBotInputText)   { *outBotInputText   = botInputText; }
}

Scene* Dialog::CreateGetStringScene(const String &msg, const String &hint)
{
    Scene *scene = GameObjectFactory::CreateUIScene();

    UIVerticalLayout *sceneVL = scene->AddComponent<UIVerticalLayout>();
    sceneVL->SetPaddings(10, 10, 10, 20);

    UILabel *msgLabel = GameObjectFactory::CreateUILabel();
    GameObject *msgGo = msgLabel->GetGameObject();
    UITextRenderer *msgText = msgLabel->GetText();
    msgText->SetWrapping(true);
    msgText->SetContent(msg);
    msgText->SetTextColor(Color::White);
    msgText->SetTextSize(12);
    msgText->SetHorizontalAlign(HorizontalAlignment::Left);
    msgText->SetVerticalAlign(VerticalAlignment::Center);

    GameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetChildrenVerticalStretch(Stretch::Full);
    buttonsHL->SetSpacing(20);
    buttonsHL->SetPaddings(5);
    UILayoutElement *buttonsGoLE = buttonsGo->AddComponent<UILayoutElement>();
    buttonsGoLE->SetFlexibleWidth(1.0f);
    buttonsGoLE->SetFlexibleHeight(0.0f);

    UIInputText *inputText = GameObjectFactory::CreateUIInputText();
    inputText->GetText()->SetContent(hint);
    UIAutoFocuser *autoFocuser = inputText->GetLabel()->GetGameObject()->
                                  AddComponent<UIAutoFocuser>();
    autoFocuser->SetFocusableToAutoFocus(inputText->GetLabel());
    Dialog::s_resultString = hint;

    UIButton *okButton = GameObjectFactory::CreateUIButton();
    okButton->GetText()->SetContent("OK");
    okButton->GetButton()->AddClickedCallback(OnAcceptButtonClicked);

    scene->SetAsChild(msgGo);
    scene->SetAsChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 10.0f) );
    scene->SetAsChild(inputText->GetGameObject());
    scene->SetAsChild(GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Flexible,
                                                         99999.0f) );
    scene->SetAsChild(buttonsGo);
    buttonsGo->SetAsChild(GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible,
                                                             0.0001f));
    buttonsGo->SetAsChild(okButton->GetGameObject());

    class GetSceneController : public Component,
                               public IValueChangedListener
    {
    public:
        UIButton *m_okButton;
        UIInputText *m_inputText;

        void OnUpdate() override
        {
            Component::OnUpdate();
            if (Input::GetKeyDown(Key::Enter)) { m_okButton->Click(); }
        }

        virtual void OnValueChanged(Object *object) override
        {
            ASSERT(m_inputText == object);
            Dialog::s_resultString = m_inputText->GetText()->GetContent();
        }
    };

    GetSceneController *gsc = inputText->GetGameObject()->AddComponent<GetSceneController>();
    gsc->m_inputText = inputText;
    gsc->m_inputText->EventEmitter<IValueChangedListener>::RegisterListener(gsc);
    gsc->m_okButton = okButton;

    return scene;
}

Scene *Dialog::CreateMsgScene(const String &msg)
{
    Scene *scene = GameObjectFactory::CreateUIScene();

    GameObject *container = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *containerL = container->AddComponent<UIVerticalLayout>();
    containerL->SetPaddings(20);

    GameObject *mainVLayoutGo = GameObjectFactory::CreateUIGameObject();
    mainVLayoutGo->AddComponent<UIVerticalLayout>();

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetMinSize( Vector2i(45) );
    UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
    icon->SetImageTexture( EDPATH("Icons/Error.png"));
    icon->GetImageTexture()->SetFilterMode( GL::FilterMode::Linear );

    GameObject *hLayoutGo = GameObjectFactory::CreateUIGameObjectNamed("HL");
    UIHorizontalLayout *hLayout = hLayoutGo->AddComponent<UIHorizontalLayout>();
    hLayout->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);
    hLayout->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    UILayoutElement *hLayoutLE = hLayoutGo->AddComponent<UILayoutElement>();
    hLayoutLE->SetPreferredSize( Vector2i::One );

    GameObject *msgGo = GameObjectFactory::CreateUIGameObjectNamed("MSGGo");
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

    UIButton *button0 = GameObjectFactory::CreateUIButton();
    button0->GetText()->SetContent("Cancel");
    button0->GetButton()->AddClickedCallback(OnAcceptButtonClicked);

    UIButton *button1 = GameObjectFactory::CreateUIButton();
    button1->GetText()->SetContent("OK");
    button1->GetButton()->AddClickedCallback(OnAcceptButtonClicked);

    scene->SetAsChild(container);
     container->SetAsChild(mainVLayoutGo);
      mainVLayoutGo->SetAsChild(hLayoutGo);
       hLayoutGo->SetAsChild(iconGo);
       hLayoutGo->SetAsChild(
          GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Min, 20));
       hLayoutGo->SetAsChild(msgGo);
      mainVLayoutGo->SetAsChild(
         GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 20));
      mainVLayoutGo->SetAsChild(buttonsGo);
       buttonsGo->SetAsChild(hSpacer);
       buttonsGo->SetAsChild(button0->GetGameObject());
       buttonsGo->SetAsChild(button1->GetGameObject());

    return scene;
}

void Dialog::AcceptDialogFile(const Path &path)
{
    Dialog::s_resultPath = path;
    Dialog::s_okPressed = true;
    EndCurrentDialog();
}

void Dialog::OnAcceptButtonClicked(IFocusable *)
{
    Dialog::s_okPressed = true;
    EndCurrentDialog();
}

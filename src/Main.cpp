/*
#include "Bang/Application.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/ProjectManager.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);

    if (argc <= 1)
    {
        Debug_Error("Please specify the project filepath as first parameter");
        return 1;
    }

    Path projectPath(argv[1]);
    ProjectManager pm;
    Project *proj = pm.OpenProject(projectPath);

    app.CreateWindow();
    proj->OpenFirstFoundScene();
    app.MainLoop();

    return 0;
}
*/
#include <iostream>

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Thread.h"
#include "Bang/Project.h"
#include "Bang/G_Image.h"
#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIButton.h"
#include "Bang/AudioClip.h"
#include "Bang/Resources.h"
#include "Bang/PointLight.h"
#include "Bang/SystemUtils.h"
#include "Bang/Application.h"
#include "Bang/MeshFactory.h"
#include "Bang/UIGameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioManager.h"
#include "Bang/UIInputText.h"
#include "Bang/SceneManager.h"
#include "Bang/SystemProcess.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/ProjectManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

class Menu : public Component
{
public:
    Menu() { }

    void OnUpdate() override
    {
        Component::OnUpdate();

        GameObject *nameGo = GameObject::Find("nameGameObject");
        GameObject *surnameGo = GameObject::Find("surnameGameObject");
        GameObject *resultGo = GameObject::Find("resultGameObject");

        String name = nameGo->GetComponent<UIInputText>()->GetText()->GetContent();
        String surname = surnameGo->GetComponent<UIInputText>()->GetText()->GetContent();
        resultGo->GetComponentInChildren<UITextRenderer>()->SetContent("Hello " + name + " " + surname);

        UIScrollArea *scrollArea = SCAST<UIScrollArea*>(
             gameObject->FindInChildren("ScrollArea")
                    ->GetComponent<UIScrollArea>() );
        Vector2i scroll = scrollArea->GetScrolling();
        if (Input::GetKeyDownRepeat(Input::Key::Left)) {
            scrollArea->SetScrolling(scroll + Vector2i::Left); }
        else if (Input::GetKeyDownRepeat(Input::Key::Right)) {
            scrollArea->SetScrolling(scroll + Vector2i::Right); }

        scroll = scrollArea->GetScrolling();
        if (Input::GetKeyDownRepeat(Input::Key::Up)) {
            scrollArea->SetScrolling(scroll + Vector2i::Up); }
        else if (Input::GetKeyDownRepeat(Input::Key::Down)) {
            scrollArea->SetScrolling(scroll + Vector2i::Down); }
    }
};

int main(int argc, char **argv)
{
    Application app(argc, argv);
    app.CreateWindow();

    /*
    Scene *scene = new Scene();
    UIGameObject *canvas = GameObjectFactory::CreateUIGameObject(true);
    canvas->AddComponent<UICanvas>();
    UIImageRenderer *bg = canvas->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::Green);
    canvas->SetParent(scene);
    UIGameObject *vLayout = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vlComp = vLayout->AddComponent<UIVerticalLayout>();

    UIGameObject *labelGo = GameObjectFactory::CreateUIGameObject();
    labelGo->GetRectTransform()->SetAnchors(Vector2(-0.5), Vector2(0.5));
    UITextRenderer *text = labelGo->AddComponent<UITextRenderer>();
    text->SetContent("Hola");
    labelGo->SetParent(vLayout);

    UIGameObject *inputTextGo = GameObjectFactory::CreateGUIInputText();
    UIInputText *inputTextComp = inputTextGo->GetComponent<UIInputText>();
    inputTextComp->GetText()->SetContent("Test 1234");
    inputTextGo->SetParent(vLayout);

    vLayout->SetParent(canvas);
    SceneManager::LoadScene(scene);
    scene->WriteToFile( EPATH("tmp/TestSimple.bscene") );
    SceneManager::LoadScene( EPATH("tmp/TestSimple.bscene") );
    */

    //*
    Scene *scene = new Scene();
    scene->AddComponent<Transform>();

    UIGameObject *rightImg = GameObjectFactory::CreateUIGameObject(true);
    rightImg->AddComponent<UIImageRenderer>()->SetTexture( EPATH("tmp/test.png") );
    UIGameObject *rightLabel = GameObjectFactory::CreateGUILabel(
                                        "Lorem ipsum dolor sit amet. El veloz "
                                        "murcielago hindu comia feliz cardillo "
                                        "y kiwi. La ciguena tocaba el saxofon "
                                        "detras del palenque de paja");
    rightLabel->GetChild("Mask")->GetComponent<UIMask>()
              ->SetMasking(false);

    UIGameObject *rightScrollArea = GameObjectFactory::CreateGUIScrollArea();
    rightScrollArea->SetName("ScrollArea");
    rightLabel->SetParent(rightScrollArea->GetComponent<UIScrollArea>()->
                          GetContainer());
    rightScrollArea->GetComponent<UIScrollArea>()->SetScrollingY(30);

    rightLabel->GetComponentInChildren<UITextRenderer>()->SetTextColor(Color::White);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Center);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetVerticalAlign(VerticalAlignment::Center);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetWrapping(true);

    UIGameObject *rightVLayout = GameObjectFactory::CreateUIGameObject(true);
    rightVLayout->AddComponent<UIVerticalLayout>();
    rightVLayout->GetRectTransform()->SetMargins(50, 10, 10, 10);
    rightImg->SetParent(rightVLayout);
    rightScrollArea->SetParent(rightVLayout);

    UIButton *buttonPlay  = GameObjectFactory::CreateGUIButton()->GetComponent<UIButton>();
    buttonPlay->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Play");
    UIButton *buttonPause = GameObjectFactory::CreateGUIButton()->GetComponent<UIButton>();
    buttonPause->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Pause");
    UIButton *buttonStop  = GameObjectFactory::CreateGUIButton()->GetComponent<UIButton>();
    buttonStop->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Stop");
    UIButton *buttonExit  = GameObjectFactory::CreateGUIButton()->GetComponent<UIButton>();
    buttonExit->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Exit");

    UIGameObject *nameLabel = GameObjectFactory::CreateGUILabel();
    nameLabel->GetComponentInChildren<UITextRenderer>()->SetContent("Name:");
    nameLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Right);
    UIGameObject *nameInput = GameObjectFactory::CreateGUIInputText();
    nameInput->SetName("nameGameObject");
    nameInput->GetComponent<UIInputText>()->GetText()->SetContent("Your name here...");
    UIGameObject *nameSubHLayout = GameObjectFactory::CreateUIGameObject(true);
    nameSubHLayout->AddComponent<UIHorizontalLayout>();
    nameLabel->SetParent(nameSubHLayout);
    nameInput->SetParent(nameSubHLayout);

    UIGameObject *surnameLabel = GameObjectFactory::CreateGUILabel();
    surnameLabel->GetComponentInChildren<UITextRenderer>()->SetContent("Surname:");
    surnameLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Right);
    UIGameObject *surnameInput = GameObjectFactory::CreateGUIInputText();
    surnameInput->SetName("surnameGameObject");
    surnameInput->GetComponent<UIInputText>()->GetText()->SetContent("Your surname here...");
    UIGameObject *surnameSubHLayout = GameObjectFactory::CreateUIGameObject(true);
    surnameSubHLayout->AddComponent<UIHorizontalLayout>();
    surnameLabel->SetParent(surnameSubHLayout);
    surnameInput->SetParent(surnameSubHLayout);

    UIGameObject *nameVLayout = GameObjectFactory::CreateUIGameObject(true);
    nameVLayout->AddComponent<UIVerticalLayout>();
    nameSubHLayout->SetParent(nameVLayout);
    surnameSubHLayout->SetParent(nameVLayout);

    UIGameObject *namesResult = GameObjectFactory::CreateGUILabel("RESULT");
    namesResult->SetName("resultGameObject");
    namesResult->GetComponentInChildren<UITextRenderer>()->SetWrapping(false);

    UIGameObject *namesVLayout = GameObjectFactory::CreateUIGameObject(true);
    namesVLayout->AddComponent<UIVerticalLayout>();
    UIImageRenderer *img4 = namesVLayout->AddComponent<UIImageRenderer>(0);
    img4->SetTint(Color::LightGray);
    nameVLayout->SetParent(namesVLayout);
    namesResult->SetParent(namesVLayout);

    UIGameObject *buttonPlayMask = GameObjectFactory::CreateUIGameObject(true);
    buttonPlayMask->AddComponent<UIMask>();
    UIImageRenderer *maskImg = buttonPlayMask->AddComponent<UIImageRenderer>();
    maskImg->SetTexture( Resources::Load<Texture2D>( EPATH("tmp/test.png") ) );
    buttonPlay->GetGameObject()->GetComponent<RectTransform>()->SetMargins(10);
    buttonPlay->GetGameObject()->SetParent(buttonPlayMask);

    UIGameObject *uiVContainer = GameObjectFactory::CreateUIGameObject(true);
    uiVContainer->GetRectTransform()->SetMargins(20);
    UIImageRenderer *uiImg = uiVContainer->AddComponent<UIImageRenderer>(0);
    uiImg->SetTint(Color::Red);

    UIGameObject *leftMask = GameObjectFactory::CreateUIGameObject(true);
    leftMask->SetName("LeftMask");
    leftMask->AddComponent<UIMask>();
    UIImageRenderer *img = leftMask->AddComponent<UIImageRenderer>();
    img->SetTexture( Resources::Load<Texture2D>( EPATH("tmp/test2.png") ) );
    UIGameObject *menuVLayout = GameObjectFactory::CreateUIGameObject(true);
    menuVLayout->AddComponent<UIVerticalLayout>();
    menuVLayout->GetRectTransform()->SetMargins(30);
    menuVLayout->GetComponent<UIVerticalLayout>()->SetSpacing(10);
    buttonPlayMask->SetParent(menuVLayout);
    buttonPause->GetGameObject()->SetParent(menuVLayout);
    buttonStop->GetGameObject()->SetParent(menuVLayout);
    buttonExit->GetGameObject()->SetParent(menuVLayout);
    menuVLayout->SetParent(leftMask);
    leftMask->SetParent(uiVContainer);

    List<UITextRenderer*> txts = menuVLayout->GetComponentsInChildren<UITextRenderer>();
    for (UITextRenderer *txt : txts)
    {
        txt->SetTextSize(20);
        txt->SetHorizontalAlign(HorizontalAlignment::Center);
        txt->SetVerticalAlign(VerticalAlignment::Center);
    }
    namesVLayout->SetParent(menuVLayout);

    int s = 10;
    nameLabel->GetComponentInChildren<UITextRenderer>()->SetTextSize(s);
    surnameLabel->GetComponentInChildren<UITextRenderer>()->SetTextSize(s);
    nameInput->GetComponent<UIInputText>()->GetText()->SetTextSize(s);
    surnameInput->GetComponent<UIInputText>()->GetText()->SetTextSize(s);

    UIGameObject *mainHLayout = GameObjectFactory::CreateUIGameObject(true);
    mainHLayout->SetName("MainHLayout");
    mainHLayout->AddComponent<Menu>();
    mainHLayout->AddComponent<UIHorizontalLayout>();
    mainHLayout->GetComponent<UIDirLayout>()->SetSpacing(0);
    rightImg->SetName("RightImg");
    rightVLayout->SetParent(mainHLayout);
    uiVContainer->SetParent(mainHLayout, 0);

    mainHLayout->GetComponent<UIDirLayout>()->SetStretch(0, 0.6f);
    mainHLayout->GetComponent<UIDirLayout>()->SetStretch(1, 0.4f);

    UIGameObject *canvas = GameObjectFactory::CreateUIGameObject(true);
    canvas->AddComponent<UICanvas>();
    mainHLayout->SetParent(canvas);
    canvas->SetParent(scene);

    AudioClip *gameAc = Resources::Load<AudioClip>(  EPATH("tmp/GameMusic.wav") );
    AudioClip *appleAc = Resources::Load<AudioClip>(  EPATH("tmp/AppleSound.wav") );
    buttonPlay->AddClickedCallback([gameAc, appleAc](UIButton*)
    {
        AudioParams ap;
        if (Input::GetKey(Input::Key::A))
        {
            AudioManager::Play(appleAc, ap);
        }
        else if (Input::GetKey(Input::Key::X))
        {
            gameAc->Import( EPATH("tmp/MenuMusic.wav") );
        }
        else
        {
            AudioManager::Play(gameAc, ap);
        }
    });
    buttonPause->AddClickedCallback([&buttonPause](UIButton*)
    {
        if (buttonPause->gameObject->GetComponentInChildren<UITextRenderer>()->GetContent().Contains("ause"))
        {
            AudioManager::PauseAllSounds();
            buttonPause->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Resume");
        }
        else
        {
            AudioManager::ResumeAllSounds();
            buttonPause->gameObject->GetComponentInChildren<UITextRenderer>()->SetContent("Pause");
        }
    });
    buttonStop->AddClickedCallback([](UIButton*)
    {
        AudioManager::StopAllSounds();
    });
    buttonExit->AddClickedCallback([](UIButton*)
    {
        exit(0);
    });

    SceneManager::LoadScene(scene);
    scene->ExportXMLToFile( EPATH("tmp/Test2.bscene") );
    // SceneManager::LoadScene( EPATH("tmp/Test2.bscene") );
    app.MainLoop();

    return 0;
}

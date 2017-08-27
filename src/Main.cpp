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
#include "Bang/GUIMask.h"
#include "Bang/Material.h"
#include "Bang/GUICanvas.h"
#include "Bang/GUIButton.h"
#include "Bang/AudioClip.h"
#include "Bang/Resources.h"
#include "Bang/PointLight.h"
#include "Bang/SystemUtils.h"
#include "Bang/Application.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioManager.h"
#include "Bang/GUIInputText.h"
#include "Bang/SceneManager.h"
#include "Bang/SystemProcess.h"
#include "Bang/GUIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/ProjectManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GUIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/GUIHorizontalLayout.h"

class Menu : public UIGameObject
{
public:
    Menu()
    {
        AddComponent<GUIHorizontalLayout>();
    }

    void OnUpdate() override
    {
        UIGameObject::OnUpdate();

        GameObject *nameGo = GameObject::Find("nameGameObject");
        GameObject *surnameGo = GameObject::Find("surnameGameObject");
        GameObject *resultGo = GameObject::Find("resultGameObject");

        String name = nameGo->GetComponent<GUIInputText>()->GetText()->GetContent();
        String surname = surnameGo->GetComponent<GUIInputText>()->GetText()->GetContent();
        resultGo->GetComponentInChildren<UITextRenderer>()->SetContent("Hello " + name + " " + surname);

        GUIScrollArea *scrollArea = SCAST<GUIScrollArea*>( FindInChildren("ScrollArea") );
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

    //*
    Scene *scene = new Scene();

    UIGameObject *rightImg = new UIGameObject();
    rightImg->AddComponent<UIImageRenderer>()->SetTexture( EPATH("tmp/test.png") );
    UIGameObject *rightLabel = GameObjectFactory::CreateGUILabel(
                                        "Lorem ipsum dolor sit amet. El veloz "
                                        "murcielago hindu comia feliz cardillo "
                                        "y kiwi. La ciguena tocaba el saxofon "
                                        "detras del palenque de paja");
    SCAST<GUIMask*>(rightLabel->GetChild("GUILabel_Mask"))->SetMasking(false);

    GUIScrollArea *rightScrollArea = new GUIScrollArea();
    rightScrollArea->SetName("ScrollArea");
    rightScrollArea->AddChild(rightLabel);
    rightScrollArea->SetScrollingY(30);

    rightLabel->GetComponentInChildren<UITextRenderer>()->SetTextColor(Color::White);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Center);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetVerticalAlign(VerticalAlignment::Center);
    rightLabel->GetComponentInChildren<UITextRenderer>()->SetWrapping(true);

    UIGameObject *rightVLayout = new UIGameObject();
    rightVLayout->AddComponent<GUIVerticalLayout>();
    rightVLayout->rectTransform->SetMargins(50, 10, 10, 10);
    rightImg->SetParent(rightVLayout);
    rightScrollArea->SetParent(rightVLayout);

    GUIButton *buttonPlay  = GameObjectFactory::CreateGUIButton()->GetComponent<GUIButton>();
    buttonPlay->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Play");
    GUIButton *buttonPause = GameObjectFactory::CreateGUIButton()->GetComponent<GUIButton>();
    buttonPause->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Pause");
    GUIButton *buttonStop  = GameObjectFactory::CreateGUIButton()->GetComponent<GUIButton>();
    buttonStop->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Stop");
    GUIButton *buttonExit  = GameObjectFactory::CreateGUIButton()->GetComponent<GUIButton>();
    buttonExit->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Exit");

    UIGameObject *nameLabel = GameObjectFactory::CreateGUILabel();
    nameLabel->GetComponentInChildren<UITextRenderer>()->SetContent("Name:");
    nameLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Right);
    UIGameObject *nameInput = GameObjectFactory::CreateGUIInputText();
    nameInput->SetName("nameGameObject");
    nameInput->GetComponent<GUIInputText>()->GetText()->SetContent("Your name here...");
    UIGameObject *nameSubHLayout = new UIGameObject();
    nameSubHLayout->AddComponent<GUIHorizontalLayout>();
    nameLabel->SetParent(nameSubHLayout);
    nameInput->SetParent(nameSubHLayout);

    UIGameObject *surnameLabel = GameObjectFactory::CreateGUILabel();
    surnameLabel->GetComponentInChildren<UITextRenderer>()->SetContent("Surname:");
    surnameLabel->GetComponentInChildren<UITextRenderer>()->SetHorizontalAlign(HorizontalAlignment::Right);
    UIGameObject *surnameInput = GameObjectFactory::CreateGUIInputText();
    surnameInput->SetName("surnameGameObject");
    surnameInput->GetComponent<GUIInputText>()->GetText()->SetContent("Your surname here...");
    UIGameObject *surnameSubHLayout = new UIGameObject();
    surnameSubHLayout->AddComponent<GUIHorizontalLayout>();
    surnameLabel->SetParent(surnameSubHLayout);
    surnameInput->SetParent(surnameSubHLayout);

    UIGameObject *nameVLayout = new UIGameObject();
    nameVLayout->AddComponent<GUIVerticalLayout>();
    nameSubHLayout->SetParent(nameVLayout);
    surnameSubHLayout->SetParent(nameVLayout);

    UIGameObject *namesResult = GameObjectFactory::CreateGUILabel("RESULT");
    namesResult->SetName("resultGameObject");
    namesResult->GetComponentInChildren<UITextRenderer>()->SetWrapping(false);

    UIGameObject *namesVLayout = new UIGameObject();
    namesVLayout->AddComponent<GUIVerticalLayout>();
    UIImageRenderer *img4 = namesVLayout->AddComponent<UIImageRenderer>(0);
    img4->SetTint(Color::LightGray);
    nameVLayout->SetParent(namesVLayout);
    namesResult->SetParent(namesVLayout);

    GUIMask *buttonPlayMask = new GUIMask();
    buttonPlayMask->SetMasking(true);
    UIImageRenderer *maskImg = buttonPlayMask->AddComponent<UIImageRenderer>();
    maskImg->SetTexture(
             new Texture2D( EPATH("tmp/test.png") ) );
    buttonPlay->GetGameObject()->rectTransform->SetMargins(10);
    buttonPlay->GetGameObject()->SetParent(buttonPlayMask);

    UIGameObject *uiVContainer = new UIGameObject();
    uiVContainer->rectTransform->SetMargins(20);
    UIImageRenderer *uiImg = uiVContainer->AddComponent<UIImageRenderer>(0);
    uiImg->SetTint(Color::Red);

    GUIMask *leftMask = new GUIMask();
    leftMask->SetMasking(true);
    UIImageRenderer *img = leftMask->AddComponent<UIImageRenderer>();
    img->SetTexture( new Texture2D( EPATH("tmp/test2.png") ) );
    UIGameObject *menuVLayout = new UIGameObject();
    menuVLayout->AddComponent<GUIVerticalLayout>();
    menuVLayout->rectTransform->SetMargins(30);
    menuVLayout->GetComponent<GUIVerticalLayout>()->SetSpacing(10);
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
    nameInput->GetComponent<GUIInputText>()->GetText()->SetTextSize(s);
    surnameInput->GetComponent<GUIInputText>()->GetText()->SetTextSize(s);

    Menu *mainHLayout = new Menu();
    mainHLayout->GetComponent<GUIDirLayout>()->SetSpacing(0);
    rightVLayout->SetParent(mainHLayout);
    uiVContainer->SetParent(mainHLayout, 0);

    mainHLayout->GetComponent<GUIDirLayout>()->SetStretch(0, 0.6f);
    mainHLayout->GetComponent<GUIDirLayout>()->SetStretch(1, 0.4f);

    UIGameObject *canvas = new UIGameObject();
    canvas->AddComponent<GUICanvas>();
    mainHLayout->SetParent(canvas);
    canvas->SetParent(scene);

    AudioClip *gameAc = new AudioClip();
    AudioClip *appleAc = new AudioClip();
    gameAc->Import( EPATH("tmp/GameMusic.wav") );
    appleAc->Import( EPATH("tmp/AppleSound.wav") );
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
        if (buttonPause->GetLabel()->GetComponentInChildren<UITextRenderer>()->GetContent().Contains("ause"))
        {
            AudioManager::PauseAllSounds();
            buttonPause->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Resume");
        }
        else
        {
            AudioManager::ResumeAllSounds();
            buttonPause->GetLabel()->GetComponentInChildren<UITextRenderer>()->SetContent("Pause");
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
    scene->WriteToFile( EPATH("tmp/Test.bscene") );
    /*/
    SceneManager::LoadScene( EPATH("tmp/Test.bscene") );
    //*/
    app.MainLoop();

    return 0;
}

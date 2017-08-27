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
#include "Bang/GUILabel.h"
#include "Bang/GUIImage.h"
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
        GUILabel *resultGo = SCAST<GUILabel*>(GameObject::Find("resultGameObject"));

        String name = SCAST<GUIInputText*>(nameGo)->GetText()->GetContent();
        String surname = SCAST<GUIInputText*>(surnameGo)->GetText()->GetContent();
        resultGo->GetText()->SetContent("Hello " + name + " " + surname);

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

    GUIImage *rightImg = new GUIImage( EPATH("tmp/test.png") );
    GUILabel *rightLabel = new GUILabel("Lorem ipsum dolor sit amet. El veloz "
                                        "murcielago hindu comia feliz cardillo "
                                        "y kiwi. La ciguena tocaba el saxofon "
                                        "detras del palenque de paja");
    rightLabel->GetMask()->SetMasking(false);

    GUIScrollArea *rightScrollArea = new GUIScrollArea();
    rightScrollArea->SetName("ScrollArea");
    rightScrollArea->AddChild(rightLabel);
    rightScrollArea->SetScrollingY(30);

    rightLabel->GetText()->SetTextColor(Color::White);
    rightLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Center);
    rightLabel->GetText()->SetVerticalAlign(VerticalAlignment::Center);
    rightLabel->GetText()->SetWrapping(true);

    UIGameObject *rightVLayout = new UIGameObject();
    rightVLayout->AddComponent<GUIVerticalLayout>();
    rightVLayout->rectTransform->SetMargins(50, 10, 10, 10);
    rightImg->SetParent(rightVLayout);
    rightScrollArea->SetParent(rightVLayout);

    GUIButton *buttonPlay = new GUIButton("Play");
    GUIButton *buttonPause= new GUIButton("Pause");
    GUIButton *buttonStop = new GUIButton("Stop");
    GUIButton *buttonExit = new GUIButton("Exit");

    GUILabel *nameLabel = new GUILabel();
    nameLabel->GetText()->SetContent("Name:");
    nameLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);
    GUIInputText *nameInput = new GUIInputText();
    nameInput->SetName("nameGameObject");
    nameInput->GetText()->SetContent("Your name here...");
    UIGameObject *nameSubHLayout = new UIGameObject();
    nameSubHLayout->AddComponent<GUIHorizontalLayout>();
    nameLabel->SetParent(nameSubHLayout);
    nameInput->SetParent(nameSubHLayout);

    GUILabel *surnameLabel = new GUILabel();
    surnameLabel->GetText()->SetContent("Surname:");
    surnameLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);
    GUIInputText *surnameInput = new GUIInputText();
    surnameInput->SetName("surnameGameObject");
    surnameInput->GetText()->SetContent("Your surname here...");
    UIGameObject *surnameSubHLayout = new UIGameObject();
    surnameSubHLayout->AddComponent<GUIHorizontalLayout>();
    surnameLabel->SetParent(surnameSubHLayout);
    surnameInput->SetParent(surnameSubHLayout);

    UIGameObject *nameVLayout = new UIGameObject();
    nameVLayout->AddComponent<GUIVerticalLayout>();
    nameSubHLayout->SetParent(nameVLayout);
    surnameSubHLayout->SetParent(nameVLayout);

    GUILabel *namesResult = new GUILabel("RESULT");
    namesResult->SetName("resultGameObject");
    namesResult->GetText()->SetWrapping(false);

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
    buttonPlay->rectTransform->SetMargins(10);
    buttonPlay->SetParent(buttonPlayMask);

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
    buttonPause->SetParent(menuVLayout);
    buttonStop->SetParent(menuVLayout);
    buttonExit->SetParent(menuVLayout);
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
    nameLabel->GetText()->SetTextSize(s);
    surnameLabel->GetText()->SetTextSize(s);
    nameInput->GetText()->SetTextSize(s);
    surnameInput->GetText()->SetTextSize(s);

    Menu *mainHLayout = new Menu();
    mainHLayout->GetComponent<GUIDirLayout>()->SetSpacing(0);
    rightVLayout->SetParent(mainHLayout);
    uiVContainer->SetParent(mainHLayout, 0);

    mainHLayout->GetComponent<GUIDirLayout>()->SetStretch(0, 0.6f);
    mainHLayout->GetComponent<GUIDirLayout>()->SetStretch(1, 0.4f);

    GUICanvas *canvas = new GUICanvas();
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
        if (buttonPause->GetLabel()->GetText()->GetContent().Contains("ause"))
        {
            AudioManager::PauseAllSounds();
            buttonPause->GetLabel()->GetText()->SetContent("Resume");
        }
        else
        {
            AudioManager::ResumeAllSounds();
            buttonPause->GetLabel()->GetText()->SetContent("Pause");
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

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

class Menu : public GUIHorizontalLayout
{
public:
    Menu() : GUIHorizontalLayout() {}

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

    {
        Scene *scene = new Scene();

        Material *mat = new Material( *MaterialFactory::GetDefault() );
        mat->SetDiffuseColor(Color::Blue);
        mat->SetTexture( Resources::Load<Texture2D>( EPATH("res/EngineAssets/Images/LogoBang_B_512.png") ) );
        mat->WriteToFile( Path("/home/sephirot47/Bang/res/EngineAssets/Materials/test.bmat") );
        Material *mat2 = Resources::Load<Material>( Path("/home/sephirot47/Bang/res/EngineAssets/Materials/test.bmat") );
        Debug_Log(mat2->GetSerializedString());

        GameObject *sphere = new GameObject("SPHERE");
        Transform *trans = sphere->AddComponent<Transform>();
        trans->SetScale(10.0f);
        MeshRenderer *mr = sphere->AddComponent<MeshRenderer>();
        mr->SetMesh( MeshFactory::GetSphere() );
        mr->UseMaterialCopy();
        mr->GetMaterial()->SetDiffuseColor(Color::Pink);
        mr->SetMaterial( mat2 );
        sphere->SetParent(scene);

        GameObject *sphere2 = sphere->Clone();
        sphere2->transform->SetPosition( Vector3(20.0f, 0.0f, 0.0f) );
        sphere2->transform->SetScale(5.0f);
        sphere2->SetParent(scene);

        GameObject *pointLightGo = new GameObject();
        Transform *transPl = pointLightGo->AddComponent<Transform>();
        PointLight *pointLight = pointLightGo->AddComponent<PointLight>();
        transPl->SetPosition( Vector3(20.0f) );
        pointLight->SetColor( Color::White );
        pointLight->SetRange(100.0f);
        pointLightGo->SetParent(scene);

        scene->WriteToFile( Path("Test.bscene") );
        SceneManager::LoadScene( Path("Test.bscene") );
        // SceneManager::LoadScene(scene);
    }
    app.MainLoop();
    return 0;

    /*
    ProjectManager pm;
    pm.OpenProject( Path("/home/sephirot47/Snake/Snake.bproject") );
    pm.GetCurrentProject()->OpenFirstFoundScene();
    exit(0);
    */

    Scene *scene = new Scene();

    GUIImage *rightImg = new GUIImage( Path("/home/sephirot47/BangUITest/test.png") );
    {
    G_Image *image = Resources::Load<G_Image>( Path("/home/sephirot47/BangUITest/test.png") );
    Texture2D *tex = Resources::Load<Texture2D>(
                Path("/home/sephirot47/BangUITest/test.png") );
    Debug_Log(image->GetSize());
    }

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

    GUIVerticalLayout *rightVLayout = new GUIVerticalLayout();
    rightVLayout->rectTransform->SetMargins(50, 10, 10, 10);
    rightVLayout->Add(rightImg);
    rightVLayout->Add(rightScrollArea);

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
    GUIHorizontalLayout *nameSubHLayout = new GUIHorizontalLayout();
    nameSubHLayout->Add(nameLabel);
    nameSubHLayout->Add(nameInput);

    GUILabel *surnameLabel = new GUILabel();
    surnameLabel->GetText()->SetContent("Surname:");
    surnameLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);
    GUIInputText *surnameInput = new GUIInputText();
    surnameInput->SetName("surnameGameObject");
    surnameInput->GetText()->SetContent("Your surname here...");
    GUIHorizontalLayout *surnameSubHLayout = new GUIHorizontalLayout();
    surnameSubHLayout->Add(surnameLabel);
    surnameSubHLayout->Add(surnameInput);

    GUIVerticalLayout *nameVLayout = new GUIVerticalLayout();
    nameVLayout->Add(nameSubHLayout);
    nameVLayout->Add(surnameSubHLayout);

    GUILabel *namesResult = new GUILabel("RESULT");
    namesResult->SetName("resultGameObject");
    namesResult->GetText()->SetWrapping(false);

    GUIVerticalLayout *namesVLayout = new GUIVerticalLayout();
    UIImageRenderer *img4 = namesVLayout->AddComponent<UIImageRenderer>(0);
    img4->SetTint(Color::LightGray);
    namesVLayout->Add(nameVLayout);
    namesVLayout->Add(namesResult);

    GUIMask *buttonPlayMask = new GUIMask();
    buttonPlayMask->SetMasking(true);
    UIImageRenderer *maskImg = buttonPlayMask->AddComponent<UIImageRenderer>();
    maskImg->SetTexture(
             new Texture2D( Path("/home/sephirot47/BangUITest/test.png") ) );
    buttonPlay->rectTransform->SetMargins(10);
    buttonPlay->SetParent(buttonPlayMask);

    G_Image *image = Resources::Load<G_Image>(Path("/home/sephirot47/BangUITest/test.png"));
    if (image) { image->SaveToFile( Path("/home/sephirot47/Bang/tmp/lol.jpg") ); }

    UIGameObject *uiVContainer = new UIGameObject();
    uiVContainer->rectTransform->SetMargins(20);
    UIImageRenderer *uiImg = uiVContainer->AddComponent<UIImageRenderer>(0);
    uiImg->SetTint(Color::Red);

    GUIMask *leftMask = new GUIMask();
    leftMask->SetMasking(true);
    UIImageRenderer *img = leftMask->AddComponent<UIImageRenderer>();
    img->SetTexture( new Texture2D( Path("/home/sephirot47/test2.png") ) );
    GUIVerticalLayout *menuVLayout = new GUIVerticalLayout();
    menuVLayout->rectTransform->SetMargins(30);
    menuVLayout->SetSpacing(10);
    menuVLayout->Add(buttonPlayMask);
    menuVLayout->Add(buttonPause);
    menuVLayout->Add(buttonStop);
    menuVLayout->Add(buttonExit);
    menuVLayout->SetParent(leftMask);
    leftMask->SetParent(uiVContainer);

    List<UITextRenderer*> txts = menuVLayout->GetComponentsInChildren<UITextRenderer>();
    for (UITextRenderer *txt : txts)
    {
        txt->SetTextSize(20);
        txt->SetHorizontalAlign(HorizontalAlignment::Center);
        txt->SetVerticalAlign(VerticalAlignment::Center);
    }
    menuVLayout->Add(namesVLayout);

    int s = 10;
    nameLabel->GetText()->SetTextSize(s);
    surnameLabel->GetText()->SetTextSize(s);
    nameInput->GetText()->SetTextSize(s);
    surnameInput->GetText()->SetTextSize(s);

    Menu *mainHLayout = new Menu();
    mainHLayout->SetSpacing(0);
    mainHLayout->Add(rightVLayout);
    mainHLayout->Add(uiVContainer, 0);

    mainHLayout->SetStretch(0, 0.6f);
    mainHLayout->SetStretch(1, 0.4f);

    GUICanvas *canvas = new GUICanvas();
    mainHLayout->SetParent(canvas);
    canvas->SetParent(scene);

    AudioClip *gameAc = new AudioClip();
    AudioClip *appleAc = new AudioClip();
    gameAc->Import( Path("/home/sephirot47/Snake/Assets/Music/GameMusic.wav") );
    appleAc->Import( Path("/home/sephirot47/Snake/Assets/Music/AppleSound.wav") );
    buttonPlay->AddClickedCallback([gameAc, appleAc](UIButton*)
    {
        AudioParams ap;
        if (Input::GetKey(Input::Key::A))
        {
            AudioManager::Play(appleAc, ap);
        }
        else if (Input::GetKey(Input::Key::X))
        {
            gameAc->Import( Path("/home/sephirot47/Snake/Assets/Music/MenuMusic.wav") );
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
    scene->WriteToFile( Path("Test.bscene") );
    app.MainLoop();

    return 0;
}

#include "Bang/Application.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/ProjectManager.h"

#include "Bang/Rect.h"
#include "Bang/Font.h"
#include "Bang/Input.h"
#include "Bang/Gizmos.h"
#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/RectTransform.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"

class GOTest : public GameObject
{
public:
    UIText *text;
    RectTransform *rt;
    GOTest()
    {
        ChangeTransformByRectTransform();
        rt = SCAST<RectTransform*>(transform);
        rt->SetAnchors(Vector2(-1), Vector2(-1));
        rt->SetMargins(0, -500, -500, 0);

        GameObject *child2 = new GameObject();
        child2->ChangeTransformByRectTransform();
        RectTransform *rt2 = child2->GetComponent<RectTransform>();
        rt2->SetMargins(5);
        rt2->SetAnchors(Vector2(-1), Vector2(1));
        child2->SetParent(this);

        GameObject *child3 = new GameObject();
        child3->ChangeTransformByRectTransform();
        RectTransform *rt3 = child3->GetComponent<RectTransform>();
        rt3->SetMargins(10);
        rt3->SetPivotPosition( Vector2(1) );
        rt3->SetAnchors(Vector2(1), Vector2(1));
        child3->SetParent(child2);

        text = AddComponent<UIText>();
        text->SetTextSize(20);
        text->SetContent("El veloz murcielago hindu comia feliz cardillo y kiwi. La ciguena tocaba el saxofon detras del palenque de paja.");
        // text->SetContent("ABC");
        text->SetHorizontalWrapMode(WrapMode::Wrap);
        text->SetVerticalWrapMode(WrapMode::Hide);
        text->SetHorizontalAlign(HorizontalAlignment::Center);
        text->SetVerticalAlign(VerticalAlignment::Center);
    }

    void OnUpdate() override
    {
        GameObject::OnUpdate();

        if (Input::GetKey(Input::Key::LControl)) { return; }

        if (Input::GetKeyDown(Input::Key::E))
        {
            WrapMode wm;
            if (text->GetHorizontalWrapMode() == WrapMode::Overflow) wm = WrapMode::Wrap;
            else if (text->GetHorizontalWrapMode() == WrapMode::Wrap) wm = WrapMode::Hide;
            else if (text->GetHorizontalWrapMode() == WrapMode::Hide) wm = WrapMode::Overflow;
            text->SetHorizontalWrapMode(wm);
        }

        if (Input::GetKeyDown(Input::Key::Q))
        {
            WrapMode wm;
            if (text->GetVerticalWrapMode() == WrapMode::Overflow) wm = WrapMode::Hide;
            else if (text->GetVerticalWrapMode() == WrapMode::Hide) wm = WrapMode::Overflow;
            text->SetVerticalWrapMode(wm);
        }

        float delta = 0.1f;
        Vector2 offset = Vector2::Zero;
        if (Input::GetKeyDown(Input::Key::A)) { offset.x = -delta; }
        if (Input::GetKeyDown(Input::Key::D)) { offset.x =  delta; }
        if (Input::GetKeyDown(Input::Key::W)) { offset.y =  delta; }
        if (Input::GetKeyDown(Input::Key::S)) { offset.y = -delta; }
        if (!Input::GetKey(Input::Key::LShift))
        {
            rt->SetAnchorMin( rt->GetAnchorMin() + offset);
        }
        else
        {
            rt->SetAnchorMax( rt->GetAnchorMax() + offset);
        }


        float textSizeOffset = 1.0f;
        if (Input::GetKeyDown(Input::Key::Up))
        {
            text->SetTextSize( text->GetTextSize() + textSizeOffset );
        }
        if (Input::GetKeyDown(Input::Key::Down))
        {
            text->SetTextSize( text->GetTextSize() - textSizeOffset );
        }

        if (Input::GetKeyDown(Input::Key::Num1))
        {
            text->SetVerticalAlign(VerticalAlignment::Top);
        }
        else if (Input::GetKeyDown(Input::Key::Num2))
        {
            text->SetVerticalAlign(VerticalAlignment::Center);
        }
        else if (Input::GetKeyDown(Input::Key::Num3))
        {
            text->SetVerticalAlign(VerticalAlignment::Bot);
        }

        if (Input::GetKeyDown(Input::Key::Num4))
        {
            text->SetHorizontalAlign(HorizontalAlignment::Left);
        }
        else if (Input::GetKeyDown(Input::Key::Num5))
        {
            text->SetHorizontalAlign(HorizontalAlignment::Center);
        }
        else if (Input::GetKeyDown(Input::Key::Num6))
        {
            text->SetHorizontalAlign(HorizontalAlignment::Right);
        }
    }
};

class BackPanel : public GameObject
{
public:
    UIImage *img;
    RectTransform *rt;
    UIText *instructionsText;
    BackPanel() : GameObject("BackPanel")
    {
        ChangeTransformByRectTransform();
        rt = SCAST<RectTransform*>(transform);
        img = AddComponent<UIImage>();
        img->SetTint(Color::Gray);

        instructionsText = AddComponent<UIText>();
        instructionsText->SetTextSize(20);
        instructionsText->SetTint(Color::Black);
        instructionsText->SetHorizontalAlign(HorizontalAlignment::Right);
        instructionsText->SetVerticalAlign(VerticalAlignment::Bot);
        instructionsText->SetContent("");
        /*instructionsText->SetContent("Instructions: \n"
                                     " - WASD: Move lower left corner.\n"
                                     " - WASD + Shift: Move top right corner.\n"
                                     " - UP/DOWN: Change text size.\n"
                                     " - E: Change Horizontal Wrap Mode.\n"
                                     " - Q: Change Vertical Wrap Mode.\n"
                                     " - 1/2/3: Change Vertical Alignment.\n"
                                     " - 4/5/6: Change Horizontal Alignment.\n");*/

        GOTest *gt = new GOTest();
        gt->SetParent(this);
    }

    void OnUpdate() override
    {
        GameObject::OnUpdate();

        if (!Input::GetKey(Input::Key::LControl)) { return; }

        float delta = 0.1f;
        Vector2 offset = Vector2::Zero;
        if (Input::GetKeyDown(Input::Key::A)) { offset.x = -delta; }
        if (Input::GetKeyDown(Input::Key::D)) { offset.x =  delta; }
        if (Input::GetKeyDown(Input::Key::W)) { offset.y =  delta; }
        if (Input::GetKeyDown(Input::Key::S)) { offset.y = -delta; }
        if (!Input::GetKey(Input::Key::LShift))
        {
            rt->SetAnchorMin( rt->GetAnchorMin() + offset);
        }
        else
        {
            rt->SetAnchorMax( rt->GetAnchorMax() + offset);
        }
    }
};

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

    // SceneManager::LoadScene( Path("Scenes/InGame2.bscene") );
    Scene *scene = SceneManager::GetActiveScene();
    for (GameObject *go : scene->GetChildren())
    {
        for (Renderer *rend : go->GetComponents<Renderer>())
        {
            rend->SetEnabled(false);
        }
        RectTransform *rt = go->GetComponent<RectTransform>();
        if (rt) { rt->SetEnabled(false); }
    }
    BackPanel *go = new BackPanel();
    go->SetParent(scene);

    app.MainLoop();

    return 0;
}

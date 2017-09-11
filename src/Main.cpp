#include "Bang/Application.h"

#include "Bang/Image.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/Image.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"

USING_NAMESPACE_BANG

int main(int argc, char **argv)
{
    Application app(argc, argv);

    if (argc <= 1)
    {
        Debug_Error("Please specify the project filepath as first parameter");
        return 1;
    }

    app.CreateWindow();

    Scene *scene = new Scene();
    scene->AddComponent<Transform>();

    UIGameObject *all = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *topHLGo  = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *topGo0 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *topGo1 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *topGo2 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *midHLGo  = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *midGo0 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *midGo1 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *midGo2 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *botHLGo  = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *botGo0 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *botGo1 = GameObjectFactory::CreateUIGameObject(true);
    UIGameObject *botGo2 = GameObjectFactory::CreateUIGameObject(true);
    RectTransform *allRT = all->GetComponent<RectTransform>();
    UIVerticalLayout *allVL = all->AddComponent<UIVerticalLayout>();
    allRT->SetMargins(30);

    UIHorizontalLayout *topHL = topHLGo->AddComponent<UIHorizontalLayout>();
    UIHorizontalLayout *midHL = midHLGo->AddComponent<UIHorizontalLayout>();
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();

    UIImageRenderer *topBg = topHLGo->AddComponent<UIImageRenderer>();
    UIImageRenderer *midBg = midHLGo->AddComponent<UIImageRenderer>();
    UIImageRenderer *botBg = botHLGo->AddComponent<UIImageRenderer>();
    UIImageRenderer *topBg0 = topGo0->AddComponent<UIImageRenderer>();
    UIImageRenderer *topBg1 = topGo1->AddComponent<UIImageRenderer>();
    UIImageRenderer *topBg2 = topGo2->AddComponent<UIImageRenderer>();
    UIImageRenderer *midBg0 = midGo0->AddComponent<UIImageRenderer>();
    UIImageRenderer *midBg1 = midGo1->AddComponent<UIImageRenderer>();
    UIImageRenderer *midBg2 = midGo2->AddComponent<UIImageRenderer>();
    UIImageRenderer *botBg0 = botGo0->AddComponent<UIImageRenderer>();
    UIImageRenderer *botBg1 = botGo1->AddComponent<UIImageRenderer>();
    UIImageRenderer *botBg2 = botGo2->AddComponent<UIImageRenderer>();

    topBg->SetTint(Color::White);
    midBg->SetTint(Color::White);
    botBg->SetTint(Color::White);

    topBg0->SetTint(Color::Red);
    topBg1->SetTint(Color::Red * 0.6);
    topBg2->SetTint(Color::Red * 0.3);
    midBg0->SetTint(Color::Green);
    midBg1->SetTint(Color::Green * 0.6);
    midBg2->SetTint(Color::Green * 0.3);
    botBg0->SetTint(Color::Blue);
    botBg1->SetTint(Color::Blue * 0.6);
    botBg2->SetTint(Color::Blue * 0.3);

    UITextRenderer *topText0 = topGo0->AddComponent<UITextRenderer>();
    UITextRenderer *topText1 = topGo1->AddComponent<UITextRenderer>();
    UITextRenderer *topText2 = topGo2->AddComponent<UITextRenderer>();
    UITextRenderer *midText0 = midGo0->AddComponent<UITextRenderer>();
    UITextRenderer *midText1 = midGo1->AddComponent<UITextRenderer>();
    UITextRenderer *midText2 = midGo2->AddComponent<UITextRenderer>();
    UITextRenderer *botText0 = botGo0->AddComponent<UITextRenderer>();
    UITextRenderer *botText1 = botGo1->AddComponent<UITextRenderer>();
    UITextRenderer *botText2 = botGo2->AddComponent<UITextRenderer>();

    all->SetParent(scene);
    topHLGo->SetParent(all);
    midHLGo->SetParent(all);
    botHLGo->SetParent(all);
    topGo0->SetParent(topHLGo);
    topGo1->SetParent(topHLGo);
    topGo2->SetParent(topHLGo);
    midGo0->SetParent(midHLGo);
    midGo1->SetParent(midHLGo);
    midGo2->SetParent(midHLGo);
    botGo0->SetParent(botHLGo);
    botGo1->SetParent(botHLGo);
    botGo2->SetParent(botHLGo);

    int size = 10;
    String sampleString = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                          "0123456789 !?#+-.,;/\\_";
    for (UITextRenderer *text : scene->GetComponentsInChildren<UITextRenderer>())
    {
        text->SetContent(sampleString);
        text->SetWrapping(true);
        text->SetHorizontalAlign(HorizontalAlignment::Center);
        text->SetVerticalAlign(VerticalAlignment::Center);
        text->SetTextSize(size);
        size *= 1.1f;
        text->SetContent("");
    }
    topText0->SetTextColor(Color::White);

    topText1->SetTextSize(12);
    topText1->SetTextColor(Color::Black);

    botText0->SetTextColor(Color::White);

    // botText1->SetBlurriness(5.0f);
    botText0->SetTextSize(7);
    botText0->SetContent(sampleString + sampleString + sampleString +
                         sampleString + sampleString + sampleString);
    botText1->SetSpacingMultiplier( Vector2(1.5f) );
    botText1->SetOutlineWidth(0.3f);
    botText1->SetOutlineBlurriness(0.05f);
    botText1->SetOutlineColor(Color::Red);
    botText1->SetContent(sampleString);
    // botText1->SetContent("0123456789");
    botText1->SetContent("azX#*/)-=jmni@");
    botText1->SetTextSize(50);

    topText1->SetContent("Bang");
    topText1->SetOutlineWidth(0.1f);
    topText1->SetTextColor(Color::Orange);
    topText1->SetOutlineBlurriness(0.02f);
    topText1->SetOutlineColor(Color::Yellow);
    topText1->SetTextSize(100);
    topText1->SetOverlapping(true);
    // botText2->SetContent("#");
    // botText2->SetTextSize(300);

    SceneManager::LoadScene(scene);

    return app.MainLoop();

    /*
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
    SceneManager::LoadScene("Tests/Test0/Test0.bscene");

    return app.MainLoop();
    */
}

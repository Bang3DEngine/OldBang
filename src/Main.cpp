#include "Bang/Application.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/ProjectManager.h"

#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/UIInputText.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIHorizontalLayout.h"
int main(int argc, char **argv)
{
    Application app(argc, argv);

    app.CreateWindow();
    Scene *scene = new Scene();

    UIGameObject *blue = new UIGameObject();
    UIImage *blueImg = blue->AddComponent<UIImage>();
    blueImg->SetTint(Color::Blue);

    UIInputText *inputText = new UIInputText();

    UIHorizontalLayout *hLayout = new UIHorizontalLayout();
    hLayout->SetParent(scene);
    hLayout->rectTransform->SetAnchors(Vector2(-0.9, -0.5), Vector2(0.9, 0.5));
    hLayout->Add(blue);
    hLayout->Add(inputText);
    hLayout->SetStretch(blue, 0.2f);

    UIVerticalLayout *vLayout = new UIVerticalLayout();
    hLayout->Add(vLayout);

    SceneManager::LoadScene(scene);
    app.MainLoop();

    return 0;
}

#include "Bang/Application.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/ProjectManager.h"

#include "Bang/UIInputText.h"
#include "Bang/SceneManager.h"
int main(int argc, char **argv)
{
    Application app(argc, argv);

    app.CreateWindow();
    Scene *scene = new Scene();
    UIInputText *inputText = new UIInputText();
    inputText->SetParent(scene);
    SceneManager::LoadScene(scene);
    app.MainLoop();

    return 0;
}

#include "Bang/Application.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/SceneManager.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);
    app.MainLoop();
    return 0;
}

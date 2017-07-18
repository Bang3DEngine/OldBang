
#include "GL/glew.h"
#include "GL/gl.h"

#include <SDL2/SDL.h>

#include "Bang/Path.h"
#include "Bang/Debug.h"
#include "Bang/Window.h"
#include "Bang/Application.h"
#include "Bang/ProjectManager.h"
#include "Bang/BehaviourManager.h"

int main(int argc, char **argv)
{
    Application app;
    app.MainLoop();
    return 0;
}

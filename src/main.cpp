
#include <vector>
#include <string>
#include <iostream>
#include <QApplication>

#include "glm/glm.hpp"

#include "ShaderProgram.h"
#include "MeshRenderer.h"
#include "MeshPyramid.h"
#include "WindowMain.h"
#include "Entity.h"
#include "Camera.h"
#include "Canvas.h"
#include "Shader.h"
#include "Timer.h"
#include "Framebuffer.h"
#include "Texture2D.h"
#include "TestBehaviour.h"
#include "StageReader.h"
#include "TestCameraBehaviour.h"

class WindowMain: public Ui_WindowMain { public: WindowMain() : Ui_WindowMain() { } };
WindowMain windowMain;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow;
    windowMain.setupUi(window);
    window->show();

    Stage *stage = new Stage();
    StageReader::ReadStage("res/Stages/stageTest1.stage", stage);
    windowMain.canvas->AddStage(stage);
    windowMain.canvas->SetCurrentStage("stageTest1");

    //stage->SetCameraChild("camera");
    ///////

    return app.exec();
}

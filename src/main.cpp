
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

    QWidget *widget = new QWidget;
    windowMain.setupUi(widget);
    widget->show();
/*
    ////////
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv,
                                          ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv);

    Texture2D *tex = new Texture2D("res/testTexture.png");
    tex->SetTextureSlot(0);

    Material *mat = new Material();
    mat->SetShaderProgram(sp);
    mat->SetTexture(tex);

    Entity *ent = new Entity("pyramid");
    Transform *t = ent->AddPart<Transform>();
    t->position = glm::vec3(0.0f, 0.0f, 0.0f);

    MeshPyramid *m = new MeshPyramid();
    MeshRenderer *mr = ent->AddPart<MeshRenderer>();
    mr->SetMesh(m);
    mr->SetMaterial(mat);
    ent->AddPart<TestBehaviour>();


    cam->AddPart<TestCameraBehaviour>();
*/


/*
    stage->AddChild(ent);
    stage->AddChild(cam);
    stage->SetCameraEntity(cam);
*/

    Stage *stage = new Stage();
    StageReader::ReadStage("res/Stages/stageTest1.stage", stage);
    windowMain.canvas->AddStage(stage);
    windowMain.canvas->SetCurrentStage("stageTest1");

    stage->SetCameraChild("camera");

    stage->GetChild("pyramid")->AddPart<TestBehaviour>();
    stage->GetChild("camera")->AddPart<TestCameraBehaviour>();
    ///////

    return app.exec();
}

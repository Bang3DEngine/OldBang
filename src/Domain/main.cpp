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
#include "TestBehaviour.h"
#include "TestCameraBehaviour.h"

class WindowMain: public Ui_WindowMain
{
public:
    WindowMain() : Ui_WindowMain()
    {
    }
};

WindowMain windowMain;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    windowMain.setupUi(widget);
    widget->show();

    MeshPyramid *m = new MeshPyramid();

    Shader *vs = new Shader(Shader::Type::Vertex);
    Shader *fs = new Shader(Shader::Type::Fragment);
    vs->LoadFromFile(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv);
    fs->LoadFromFile(ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv);

    ShaderProgram *sp = new ShaderProgram();
    sp->BindVertexShader(vs);
    sp->BindFragmentShader(fs);
    sp->Link();

    Material *mat = new Material();
    mat->SetShaderProgram(sp);

    Stage *stage = windowMain.canvas->AddStage("testStage");

    Entity *ent = new Entity("pyramid");
    stage->AddChild(ent);
    Transform *t = ent->AddPart<Transform>();
    t->position = glm::vec3(0.0f, 0.0f, 0.0f);
    //t->rotation = glm::angleAxis(3.141592f/2, glm::vec3(0.0f, 1.0f, 0.0f));
    MeshRenderer *mr = ent->AddPart<MeshRenderer>();
    mr->SetMesh(m);
    mr->SetMaterial(mat);
    TestBehaviour *testBehaviour = ent->AddPart<TestBehaviour>();

    Entity *cam = new Entity("camera");
    Camera *camPart = cam->AddPart<Camera>();
    Transform *t2 = cam->AddPart<Transform>();
    t2->position = glm::vec3(0.0f, 1.0f, 3.0f);
    //t2->rotation = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 3.0f));

    cam->AddPart<TestCameraBehaviour>();

    stage->AddChild(cam);
    stage->SetCameraEntity(cam);

    windowMain.canvas->SetCurrentStage("testStage");

    return app.exec();
}

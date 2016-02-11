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

    ////////
    MeshPyramid *m = new MeshPyramid();

    Shader *vs = new Shader(Shader::Type::Vertex);
    Shader *fs = new Shader(Shader::Type::Fragment);
    vs->LoadFromFile(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv);
    fs->LoadFromFile(ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv);

    Material *mat = new Material();

    ShaderProgram *sp = new ShaderProgram();
    sp->BindVertexShader(vs);
    sp->BindFragmentShader(fs);
    sp->Link();
    mat->SetShaderProgram(sp);

    Texture *tex = new Texture(1);
    tex->LoadFromFile("res/testTexture.png");
    mat->SetTexture(tex);

    Stage *stage = windowMain.canvas->AddStage("testStage");

    Entity *ent = new Entity("pyramid");
    stage->AddChild(ent);
    Transform *t = ent->AddPart<Transform>();
    t->position = glm::vec3(0.0f, 0.0f, 0.0f);

    MeshRenderer *mr = ent->AddPart<MeshRenderer>();
    mr->SetMesh(m);
    mr->SetMaterial(mat);
    TestBehaviour *testBehaviour = ent->AddPart<TestBehaviour>();

    Entity *cam = new Entity("camera");
    Camera *camPart = cam->AddPart<Camera>();
    Transform *t2 = cam->AddPart<Transform>();
    t2->position = glm::vec3(0.0f, 1.0f, 2.0f);

    cam->AddPart<TestCameraBehaviour>();

    stage->AddChild(cam);
    stage->SetCameraEntity(cam);

    windowMain.canvas->SetCurrentStage("testStage");

    Framebuffer *fb = new Framebuffer(Canvas::GetWidth(), Canvas::GetHeight());
    fb->CreateDepthBufferAttachment();
    fb->CreateTextureAttachment(0);
    //fb->Bind();
    ///////

    return app.exec();
}

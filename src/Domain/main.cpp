#include <vector>
#include <string>
#include <iostream>
#include <QApplication>

#include "glm/glm.hpp"

#include "ShaderProgram.h"
#include "MeshRenderer.h"
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

std::vector<glm::vec3> pyramid =
{
    //RIGHT
    glm::vec3(0.0f,  0.5f,  0.0f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),

    //LEFT
    glm::vec3( 0.0f,  0.5f,  0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),

    //FRONT
    glm::vec3( 0.0f,  0.5f, 0.0f),
    glm::vec3(-0.5f, -0.5f, 0.5f),
    glm::vec3( 0.5f, -0.5f, 0.5f),

    //BACK
    glm::vec3( 0.0f,  0.5f,  0.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),

    //BOTTOM FRONT
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),

    //BOTTOM BACK
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f)
};

std::vector<glm::vec3> normals =
{
    //RIGHT
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),

    //LEFT
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),

    //FRONT
    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -1.0f),

    //BACK
    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),

    //BOTTOM FRONT
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),

    //BOTTOM BACK
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f)
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    windowMain.setupUi(widget);
    widget->show();

    Mesh *m = new Mesh();
    m->LoadVertices(pyramid);
    m->LoadNormals(normals);

    Shader *vs = new Shader(Shader::Type::Vertex);
    Shader *fs = new Shader(Shader::Type::Fragment);
    vs->LoadFromFile(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal);
    fs->LoadFromFile(ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal);

    ShaderProgram *sp = new ShaderProgram();
    sp->BindVertexShader(vs);
    sp->BindFragmentShader(fs);
    sp->Link();

    Material *mat = new Material();
    mat->SetShaderProgram(sp);

    Stage *stage = windowMain.canvas->AddStage("testStage");

    Entity *ent = new Entity("test");
    stage->AddChild(ent);
    Transform *t = ent->AddPart<Transform>();
    t->position = glm::vec3(0.0f, 0.0f, 0.0f);
    t->rotation = glm::angleAxis(60.0f, glm::vec3(0.0f, 1.0f, 0.7f));
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

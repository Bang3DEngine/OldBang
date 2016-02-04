#include <vector>
#include <string>
#include <iostream>
#include <QApplication>

#include "glm/glm.hpp"

#include "ShaderProgram.h"
#include "MeshRenderer.h"
#include "WindowMain.h"
#include "Canvas.h"
#include "Shader.h"


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
    glm::vec3(0.0f,  0.5f,  0.0f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),

    glm::vec3( 0.0f,  0.5f,  0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),

    glm::vec3( 0.0f,  0.5f, 0.0f),
    glm::vec3(-0.5f, -0.5f, 0.5f),
    glm::vec3( 0.5f, -0.5f, 0.5f),

    glm::vec3( 0.0f,  0.5f,  0.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),

    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),

    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f)
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    windowMain.setupUi(widget);
    widget->show();

    Mesh *m = new Mesh();
    m->Load(pyramid);

    Shader *vs = new Shader(Shader::Type::Vertex);
    Shader *fs = new Shader(Shader::Type::Fragment);
    vs->LoadFromFile("res/Shaders/pass.vert");
    fs->LoadFromFile("res/Shaders/pass.frag");

    ShaderProgram *sp = new ShaderProgram();
    sp->BindVertexShader(vs);
    sp->BindFragmentShader(fs);
    sp->Link();

    Material *mat = new Material();
    mat->SetShaderProgram(sp);

    MeshRenderer *mr = new MeshRenderer();
    mr->SetMesh(m);
    mr->SetMaterial(mat);

    Transform t;
    t.position = glm::vec3(0.0f, 0.3f, 0.0f);

    windowMain.canvas->Draw(mr, &t);

    return app.exec();
}

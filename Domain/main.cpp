#include <vector>
#include <string>
#include <iostream>
#include <QApplication>
#include "View/WindowMain.h"
#include "View/Canvas.h"
#include "Domain/Shader.h"

#include "Domain/glm/glm.hpp"

#include "ShaderProgram.h"
#include "MeshRenderer.h"

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

    Shader *vs = new Shader(Shader::Type::Vertex);
    Shader *fs = new Shader(Shader::Type::Fragment);
    vs->LoadFromFile("Domain/Shaders/pass.vert");
    fs->LoadFromFile("Domain/Shaders/pass.frag");

    ShaderProgram *sp = new ShaderProgram();
    sp->BindVertexShader(vs);
    sp->BindFragmentShader(fs);
    sp->Link();

    Mesh *m = new Mesh();
    m->Load(pyramid);

    MeshRenderer *mr = new MeshRenderer();
    mr->SetMesh(m);

    windowMain.canvas->Draw(mr, sp);

    return app.exec();
}

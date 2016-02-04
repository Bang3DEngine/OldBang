#include <iostream>
#include <QApplication>
#include "View/WindowMain.h"
#include "View/Canvas.h"
#include "Domain/Shader.h"

#include "Domain/glm/glm.hpp"

#include "ShaderProgram.h"
#include "VAO.h"

class WindowMain: public Ui_WindowMain
{
public:
    WindowMain() : Ui_WindowMain()
    {
    }
};

WindowMain windowMain;

glm::vec3 pyramid[] =
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

    VBO vbo;
    vbo.Fill(pyramid, sizeof(pyramid));

    VAO vao;
    vao.BindVBO(vbo, 3);

    Shader vs(Shader::Type::Vertex);
    Shader fs(Shader::Type::Fragment);
    vs.LoadFromFile("Domain/Shaders/pass.vert");
    fs.LoadFromFile("Domain/Shaders/pass.frag");

    ShaderProgram sp;
    sp.BindVertexShader(&vs);
    sp.BindFragmentShader(&fs);
    sp.Link();

    windowMain.canvas->vaosToDraw.push_back(&vao);
    windowMain.canvas->shaderProgramsToDraw.push_back(&sp);
    windowMain.canvas->vertexCountsToDraw.push_back(sizeof(pyramid) / (sizeof(float) * 3));

    return app.exec();
}

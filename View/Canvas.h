#ifndef CANVAS_H
#define CANVAS_H

#include "../Domain/VAO.h"
#include "../Domain/Shader.h"
#include "../Domain/ShaderProgram.h"

#include <vector>
#include <string>
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT
public:

    std::vector<VAO*> vaosToDraw;
    std::vector<ShaderProgram*> shaderProgramsToDraw;
    std::vector<int> vertexCountsToDraw;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:

public slots:

};

#endif // CANVAS_H

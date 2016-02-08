#ifndef CANVAS_H
#define CANVAS_H

#include "VAO.h"
#include "Shader.h"
#include "Stage.h"
#include "ShaderProgram.h"
#include "MeshRenderer.h"

#include <vector>
#include <string>
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT

private:
    static float lastRenderTime, deltaTime;

    std::list<Stage*> stages;

    Stage *currentStage;
    
public:

    glm::vec4 clearColor;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    Stage* AddStage(const std::string &name);
    void SetStage(const std::string &name);
    Stage* GetStage(const std::string &name) const;
    void RemoveStage(const std::string &name);

    static float GetDeltaTime() { return deltaTime; }

signals:

public slots:

};

#endif // CANVAS_H

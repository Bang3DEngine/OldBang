#ifndef CANVAS_H
#define CANVAS_H

#include "VAO.h"
#include "Shader.h"
#include "Stage.h"
#include "Time.h"
#include "ShaderProgram.h"
#include "MeshRenderer.h"

#include <vector>
#include <string>
#include <QGLWidget>
#include <chrono>
#include <QTimer>

class Canvas : public QGLWidget
{
    Q_OBJECT

private:
    static int RedrawDelay;
    static float aspectRatio;
    static int width, height;
    static unsigned long long lastRenderTime;

    std::list<Stage*> stages;

    Stage *currentStage;
    QTimer drawTimer;

public:

    glm::vec4 clearColor;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    Stage* AddStage(const std::string &name);
    void SetCurrentStage(const std::string &name);
    Stage* GetCurrentStage() const;
    Stage* GetStage(const std::string &name) const;
    void RemoveStage(const std::string &name);

    static float GetAspectRatio();
    static int GetWidth();
    static int GetHeight();

signals:

public slots:
    void updateGL() override;

};

#endif // CANVAS_H

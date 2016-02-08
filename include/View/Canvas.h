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
#include <chrono>
#include <QTimer>

class Canvas : public QGLWidget
{
    Q_OBJECT

private:
    static int RedrawDelay;
    static unsigned long long lastRenderTime;

    std::list<Stage*> stages;

    Stage *currentStage;
    QTimer drawTimer;

    static unsigned long long GetNow() {
        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    }

public:
    static double deltaTime;

    glm::vec4 clearColor;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    Stage* AddStage(const std::string &name);
    void SetStage(const std::string &name);
    Stage* GetStage(const std::string &name) const;
    void RemoveStage(const std::string &name);

    static float GetDeltaTime() { return deltaTime; }

signals:

public slots:
    void updateGL() override;

};

#endif // CANVAS_H

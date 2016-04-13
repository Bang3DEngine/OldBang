#ifndef CANVAS_H
#define CANVAS_H

#include "Bang.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <chrono>
#include <QTimer>
#include <QGLWidget>
#include <QApplication>

#include "VAO.h"
#include "Time.h"
#include "Input.h"
#include "Stage.h"
#include "Prefab.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "IWindowEventManagerListener.h"

class WindowMain;
class Canvas : public QGLWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    static int RedrawDelay;
    static float aspectRatio;
    static int width, height;
    static unsigned long long lastRenderTime;

    bool paused = false;
    std::list<Stage*> stages;

    Stage *currentStage = nullptr;
    QTimer drawTimer;

public:

    glm::vec4 clearColor;
    WindowMain *windowMain = nullptr;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    Stage* AddStage(const std::string &name);
    void AddStage(Stage* stage);
    void SetCurrentStage(const std::string &name);
    Stage* GetCurrentStage() const;
    Stage* GetStage(const std::string &name) const;
    void RemoveStage(const std::string &name);

    static float GetAspectRatio();
    static int GetWidth();
    static int GetHeight();

    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public slots:
    void updateGL() override;
    void OnTopKekPressed();
    void OnPauseResumeButtonPressed();
};

#endif // CANVAS_H

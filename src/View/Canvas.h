#ifndef CANVAS_H
#define CANVAS_H

#include <QThread>

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
#include "Scene.h"
#include "Prefab.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "IWindowEventManagerListener.h"

class WindowMain;
class SelectionFramebuffer;
class Canvas : public QGLWidget, public IWindowEventManagerListener
{
    Q_OBJECT

private:
    static int RedrawDelay;
    static Canvas *stCanvas;
    static float aspectRatio;
    static int width, height;
    static unsigned long long lastRenderTime;

    bool paused = false;
    std::list<Scene*> scenes;

    Scene *currentScene = nullptr;
    QTimer drawTimer;

public:

    glm::vec4 clearColor;
    WindowMain *windowMain = nullptr;

    explicit Canvas(QWidget *parent = 0);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    Scene* AddScene(const std::string &name);
    void AddScene(Scene* scene);
    void SetCurrentScene(Scene *scene);
    void SetCurrentScene(const std::string &name);
    void RemoveScene(const std::string &name);

    static Canvas *GetInstance();
    Scene* GetCurrentScene() const;
    Scene* GetScene(const std::string &name) const;
    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();

    static void SetCursor(Qt::CursorShape cs);

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

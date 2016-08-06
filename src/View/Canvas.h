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

class WindowMain;
class GameWindow;
class SelectionFramebuffer;
class Canvas : public QGLWidget
{
    Q_OBJECT

private:

    static Canvas *m_mainBinaryCanvas;

    #ifdef BANG_EDITOR
    static WindowMain *s_m_window;
    #else
    static GameWindow *s_m_window;
    #endif

    const int c_redrawDelay = 15;

    int m_width = 0;
    int m_height = 0;
    float m_aspectRatio = 1.0f;
    unsigned long long m_lastRenderTime = 0;

    std::list<Scene*> m_scenes;

    Scene *m_currentScene = nullptr;
    QTimer m_drawTimer;

public:

    Color m_clearColor = Color::blue * 0.6f;

    explicit Canvas(QWidget *parent = 0);
    static void InitFromMainBinary();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    Scene* AddScene(const std::string &name);
    void AddScene(Scene* scene);
    void SetCurrentScene(Scene *scene);
    void SetCurrentScene(const std::string &name);
    void RemoveScene(const std::string &name);

    static Scene* GetCurrentScene();
    Scene* GetScene(const std::string &name) const;
    static Canvas *GetInstance();
    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();

    static void SetCursor(Qt::CursorShape cs);


    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public slots:
    void updateGL() override;
};

#endif // CANVAS_H

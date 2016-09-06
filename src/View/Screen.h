#ifndef CANVAS_H
#define CANVAS_H

#include <GL/glew.h>

#include <QThread>

#include "Array.h"
#include <chrono>
#include <QTimer>
#include <QGLWidget>
#include <QApplication>

#include "Bang.h"

#include "VAO.h"
#include "Time.h"
#include "Input.h"
#include "Scene.h"
#include "Prefab.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"

#ifdef BANG_EDITOR
#include "IDragDropListener.h"
#include "IShortcutListener.h"
#else
class IDragDropListener { }; // To solve a Qt moc bug
class IShortcutListener { }; // To solve a Qt moc bug
#endif

class EditorWindow;
class GameWindow;
class SelectionFramebuffer;
class Screen : public QGLWidget,
               public IDragDropListener, // Must be in non-editor compile too. Weird QT moc bug
               public IShortcutListener
{
    Q_OBJECT

private:

    static Screen *m_mainBinaryScreen;

    #ifdef BANG_EDITOR
    static EditorWindow *s_m_window;
    #else
    static GameWindow *s_m_window;
    #endif


    QWidget *m_dragOrigin = nullptr;

    int m_width = 0;
    int m_height = 0;
    float m_aspectRatio = 1.0f;

    GameObject *m_lastGameObjectOvered = nullptr;
    GameObject *m_gameObjectBeingDragged = nullptr;

    void HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin);

public:

    explicit Screen(QWidget *parent = 0);
    static void InitFromMainBinary();

    void initializeGL() override;
    void paintGL() override;
    void Render();
    void resizeGL(int w, int h) override;

    static Screen *GetInstance();
    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();
    static Vector2 GetSize();

    static void SetCursor(Qt::CursorShape cs);


    #ifdef BANG_EDITOR
    void OnShortcutPressed() override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void OnDrop(const DragDropInfo &ddi) override;
    #endif

    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // CANVAS_H

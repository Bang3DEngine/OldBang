#ifndef SCREEN_H
#define SCREEN_H

#include <GL/glew.h>

#include <chrono>
#include <QTimer>
#include <QThread>
#include <QGLWidget>
#include <QApplication>

#include "Array.h"

#ifdef BANG_EDITOR
#include "IDragDropListener.h"
#include "IShortcutListener.h"
#else
class IDragDropListener { }; // To solve a Qt moc bug
class IShortcutListener { }; // To solve a Qt moc bug
#endif

class GameObject;
class GameWindow;
class EditorWindow;
class GraphicPipeline;
class SelectionFramebuffer;
class Screen : public QGLWidget,
               public IDragDropListener, // Must be in non-editor compile too. Weird QT moc bug
               public IShortcutListener
{
    Q_OBJECT

public:
    explicit Screen(QWidget *parent = 0);
    virtual ~Screen();

    void initializeGL() override;
    void paintGL() override;
    void Render();
    void resizeGL(int w, int h) override;

    bool IsRenderingInGame() const;

    static Screen *GetInstance();
    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();
    static Vector2 GetSize();
    static Vector2 GetPixelClipSize();
    static bool MouseOver();

    GraphicPipeline *GetGraphicPipeline() const;

    #ifdef BANG_EDITOR
    void OnShortcutPressed() override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void OnDrop(const DragDropInfo &ddi) override;
    #endif

private:
    QWidget *m_dragOrigin = nullptr;
    GraphicPipeline *m_gPipeline = nullptr;

    int m_width = 1;
    int m_height = 1;
    float m_aspectRatio = 1.0f;

    GameObject *m_lastGameObjectOvered = nullptr;
    GameObject *m_gameObjectBeingDragged = nullptr;

    void HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin);

    friend class GraphicPipeline;
};

#endif // SCREEN_H

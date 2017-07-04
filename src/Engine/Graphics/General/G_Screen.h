#ifndef SCREEN_H
#define SCREEN_H

#include <GL/glew.h>

#include <chrono>
#include <QTimer>
#include <QThread>
#include <QGLWidget>
#include <QApplication>
#include "Bang/WinUndef.h"

#include "Bang/Array.h"
#include "Bang/Shortcut.h"
#include "Bang/IDragDropListener.h"

class GameObject;
class GameWindow;
class EditorWindow;
class G_GraphicPipeline;
class SelectionFramebuffer;
class G_Screen : public QGLWidget,
                 public IDragDropListener
{
    Q_OBJECT

public:
    explicit G_Screen(QWidget *parent = 0);
    virtual ~G_Screen();

    void initializeGL() override;
    void paintGL() override;
    void Render();
    void resizeGL(int w, int h) override;

    bool IsRenderingInGame() const;

    static G_Screen *GetInstance();
    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();
    static Vector2 GetSize();
    static Vector2 GetPixelClipSize();
    static bool IsMouseOver();

    G_GraphicPipeline *GetGraphicPipeline() const;

    #ifdef BANG_EDITOR
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void OnDrop(const DragDropInfo &ddi) override;
    #endif

public slots:
    void OnCopyClicked();
    void OnPasteClicked();
    void OnDuplicateClicked();
    void OnDeleteClicked();

private:
    LocalShortcut m_copyShortcut, m_pasteShortcut,
                  m_duplicateShortcut, m_deleteShortcut;

    QWidget *m_dragOrigin = nullptr;
    G_GraphicPipeline *m_gPipeline = nullptr;

    int m_width = 1;
    int m_height = 1;
    float m_aspectRatio = 1.0f;

    GameObject *m_lastGameObjectOvered = nullptr;
    GameObject *m_gameObjectBeingDragged = nullptr;

    #ifdef BANG_EDITOR
    void HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin);
    #endif

    friend class G_GraphicPipeline;
};

#endif // SCREEN_H

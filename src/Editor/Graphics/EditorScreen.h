#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "Bang/Screen.h"
#include "Bang/Shortcut.h"
#include "Bang/IDragDropListener.h"

class GameObject;
class GraphicPipeline;
class EditorScreen : public Screen,
                     public IDragDropListener
{
    Q_OBJECT

public:
    EditorScreen(QWidget *parent = nullptr);
    virtual ~EditorScreen();

    void Render() const override;

    bool IsRenderingInGame() const;

    bool IsMouseOver() const;

    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void OnDrop(const DragDropInfo &ddi) override;

public slots:
    void OnCopyClicked();
    void OnPasteClicked();
    void OnDuplicateClicked();
    void OnDeleteClicked();

private:
    LocalShortcut m_copyShortcut, m_pasteShortcut,
                  m_duplicateShortcut, m_deleteShortcut;

    QWidget *m_dragOrigin = nullptr;

    GameObject *m_lastGameObjectOvered = nullptr;
    GameObject *m_gameObjectBeingDragged = nullptr;

    void HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin);
};

#endif // EDITORSCREEN_H

#ifndef IDRAGDROPLISTENER_H
#define IDRAGDROPLISTENER_H

#include <QWidget>
#include <QListView>
#include <QTreeWidget>
#include <QMainWindow>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>

#include "Color.h"
#include "Vector2.h"

#ifdef BANG_EDITOR
#include "DragDropManager.h"
#endif

/**
 * @brief Listen to drag/drop events
 */
class IDragDropListener
{
public:
    static QString acceptDragStyle, acceptDragStyleOnlyBorder,
                   overDragStyle, rejectDragStyle;

    IDragDropListener();
    virtual ~IDragDropListener();

protected:

    // User should override these methods
    virtual void OnDragStart(const DragDropInfo &ddi)     {} // Drag Starts
    virtual void OnDragEnter(const DragDropInfo &ddi)     {} // Enters a widget
    virtual void OnDragMove(const DragDropInfo &ddi)      {} // Moves over a widget
    virtual void OnDragLeave(const DragDropInfo &ddi)     {} // Leaves a widget
    virtual void OnDrop(const DragDropInfo &ddi)          {} // Drag Ends

    bool MouseOverMe() const;
    static bool MouseOver(QWidget *w);

    friend class DragDropManager;
    friend class DragDropAgent;
};

#endif // IDRAGDROPLISTENER_H

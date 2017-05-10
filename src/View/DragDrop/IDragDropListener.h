#ifndef IDRAGDROPLISTENER_H
#define IDRAGDROPLISTENER_H

#ifdef BANG_EDITOR

#include <QWidget>
#include <QListView>
#include <QTreeWidget>
#include <QMainWindow>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include "Bang/WinUndef.h"

#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/DragDropInfo.h"

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

    bool IsMouseOverMe() const;
    static bool IsMouseOver(const QWidget *w);

    friend class DragDropManager;
    friend class DragDropAgent;
};

#else

class IDragDropListener { }; // To solve a Qt moc stuff

#endif

#endif // IDRAGDROPLISTENER_H

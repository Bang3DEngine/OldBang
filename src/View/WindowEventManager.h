#ifndef WINDOWEVENTMANAGER_H
#define WINDOWEVENTMANAGER_H

#include "Bang.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <chrono>
#include <QTimer>
#include <QGLWidget>
#include <QApplication>
#include <QTreeWidgetItem>

#include "MenuBar.h"
#include "InspectorWidget.h"
#include "InspectorWidgetInfo.h"
#include "IWindowEventManagerListener.h"

#define PROPAGATE_EVENT_PAR(FUNCTION, ITERABLE) do {\
    for(auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        if((*it) ) (*it)->FUNCTION;\
    }\
} while(0)


class Component;
class GameObject;
class WindowEventManager : public QGLWidget
{
    Q_OBJECT
private:
    static WindowEventManager *wem;
    std::list<IWindowEventManagerListener*> listeners;

public:
    explicit WindowEventManager(QWidget *parent = 0);

    void AddListener(IWindowEventManagerListener *listener);
    void RemoveListener(IWindowEventManagerListener *listener);

    static WindowEventManager *GetInstance();


public slots:

    static void NotifyChildAdded(GameObject *child);
    static void NotifyChildChangedParent(GameObject *child, GameObject *previousParent);
    static void NotifyChildRemoved(GameObject *child);

    static void NotifyMenuBarActionClicked(MenuBar::Action clickedAction);

    static void NotifyButtonTranslateModeSelected();
    static void NotifyButtonRotateModeSelected();
    static void NotifyButtonScaleModeSelected();

    static void NotifyInspectorSlotChanged(Component *updatedComponent, InspectorWidget *inspectorItem);
    static void NotifyHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects);
    static void NotifyHierarchyGameObjectDoubleClicked(GameObject *selected);
};

#endif // WINDOWEVENTMANAGER_H

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
#include "XMLNode.h"

#define PROPAGATE_EVENT_PAR(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        if ((*it) ) (*it)->FUNCTION;\
    }\
} while (0)


class Component;
class GameObject;
class InspectorWidget;
class IWindowEventManagerListener;
class WindowEventManager : public QGLWidget
{
    Q_OBJECT
private:
    static WindowEventManager *s_wem;
    std::list<IWindowEventManagerListener*> m_listeners;

public:
    explicit WindowEventManager(QWidget *parent = 0);

    void AddListener(IWindowEventManagerListener *listener);
    void RemoveListener(IWindowEventManagerListener *listener);

    static WindowEventManager *GetInstance();


public slots:

    static void NotifyMenuBarActionClicked(MenuBar::Action clickedAction);

    static void NotifyButtonTranslateModeSelected();
    static void NotifyButtonRotateModeSelected();
    static void NotifyButtonScaleModeSelected();

    static void NotifyInspectorSlotChanged(InspectorWidget *inspectorItem);
    static void NotifyHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects);
    static void NotifyHierarchyGameObjectDoubleClicked(GameObject *selected);
};

#endif // WINDOWEVENTMANAGER_H

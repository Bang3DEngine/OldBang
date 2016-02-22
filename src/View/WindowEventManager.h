#ifndef WINDOWEVENTMANAGER_H
#define WINDOWEVENTMANAGER_H

#include <GL/glew.h>

#include <vector>
#include <string>
#include <QGLWidget>
#include <chrono>
#include <QTimer>
#include <QApplication>

#include "IWindowEventManagerListener.h"

class WindowEventManager : public QGLWidget
{
    Q_OBJECT
private:
    static WindowEventManager *wem;
    std::vector<IWindowEventManagerListener*> listeners;

public:
    explicit WindowEventManager(QWidget *parent = 0);

    void AddListener(IWindowEventManagerListener *listener);

    static WindowEventManager *GetInstance();


public slots:

    static void OnMenuBarItemClicked(int itemClcked);
    void _OnMenuBarItemClicked(int itemClicked);
};

#endif // WINDOWEVENTMANAGER_H

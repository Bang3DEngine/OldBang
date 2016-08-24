#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <map>

#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

class Scene;
class SceneManager;
class ShortcutManager;
class Application : public QApplication
{
    Q_OBJECT

friend class SceneManager;
friend class ShortcutManager;
friend class IShortcutListener;

private:
    SceneManager *m_sceneManager = nullptr;

    unsigned long long m_lastRenderTime = 0;
    const int c_redrawDelay = 25;
    QTimer m_drawTimer;


public:
    Application(int& argc, char** argv);

    static Application *GetInstance();
    virtual bool notify(QObject *receiver, QEvent *e) override;

public slots:
    void OnDrawTimerTick();
};

#endif // APPLICATION_H

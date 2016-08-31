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
class AssetsManager;
class ShortcutManager;
class BehaviourManager;
class Application : public QApplication
{
    Q_OBJECT

friend class SceneManager;
friend class ShortcutManager;
friend class BehaviourManager;
friend class IShortcutListener;

private:

    SceneManager *m_sceneManager = nullptr;
    AssetsManager *m_assetsManager = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    unsigned long long m_lastRenderTime = 0;
    const int c_redrawDelay = 30;
    QTimer m_drawTimer;


public:
    Application(int& argc, char** argv);

    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    virtual bool notify(QObject *receiver, QEvent *e) override;

public slots:
    void OnDrawTimerTick();
};

#endif // APPLICATION_H

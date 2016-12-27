#ifndef APPLICATION_H
#define APPLICATION_H


#include <QTimer>
#include <iostream>
#include <QKeyEvent>
#include <QApplication>

#include "Map.h"
#include "Array.h"

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

    // To detect when it's autorepeat (very fast Release after a Press)
    struct LastKeyEventInfo
    {
        unsigned long long time;
        int key;
    };
    LastKeyEventInfo m_lastKeyPressEvInfo;

public:
    Application(int& argc, char** argv);

    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();

    bool notify(QObject *receiver, QEvent *e) override;

private slots:
    bool CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent);

public slots:
    void OnDrawTimerTick();
};

#endif // APPLICATION_H

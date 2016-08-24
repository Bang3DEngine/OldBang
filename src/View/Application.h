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

    /**
     * @brief Useful for Input processing, to delay the events not being handled before the
     * update loop. This avoids handling events between the OnUpdate and Input->OnNewFrame()
     * and losing them.
     */
    bool m_delayEventsForNextFrame = true;

    unsigned long long m_lastRenderTime = 0;
    const int c_redrawDelay = 25;
    QTimer m_drawTimer;


public:
    Application(int& argc, char** argv);

    static Application *GetInstance();
    virtual bool notify(QObject *receiver, QEvent *e) override;

    bool DelayEventsForNextFrame() const;

public slots:
    void OnDrawTimerTick();
};

#endif // APPLICATION_H

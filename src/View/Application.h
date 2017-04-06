#ifndef APPLICATION_H
#define APPLICATION_H

#include <QTimer>
#include <iostream>
#include <QKeyEvent>
#include <QApplication>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Array.h"

#ifdef BANG_EDITOR
#include "Bang/FileTracker.h"
#endif

class Scene;
class AudioManager;
class SceneManager;
class AssetsManager;
class BehaviourManager;
class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    virtual ~Application();

    void InitManagers();
    void MainLoop();

    AudioManager *GetAudioManager() const;
    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    void ResetDeltaTime();

    bool notify(QObject *receiver, QEvent *e) override;

private:
    #ifdef BANG_EDITOR
    FileTracker *m_fileTracker           = nullptr;
    #endif

    AudioManager *m_audioManager         = nullptr;
    SceneManager *m_sceneManager         = nullptr;
    AssetsManager *m_assetsManager       = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    unsigned long long m_lastRenderTime = 0;

    // If this is too low, Qt stops it when dragging
    // something around the window :(
    const int c_redrawDelay = 16; // 60 FPS cap
    //const int c_redrawDelay = 35; // 30 FPS cap

    // To detect when it's autorepeat (very fast Release after a Press)
    struct LastKeyEventInfo
    {
        unsigned long long time;
        int key;
    };
    LastKeyEventInfo m_lastKeyPressEvInfo;

private slots:
    bool CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent);

    friend class SceneManager;
    friend class BehaviourManager;
};

#endif // APPLICATION_H

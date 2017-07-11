#ifndef APPLICATION_H
#define APPLICATION_H

#include <QTimer>
#include <iostream>
#include <QKeyEvent>
#include <QApplication>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Array.h"

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

    virtual void InitManagers();
    virtual void MainLoop();

    AudioManager *GetAudioManager() const;
    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();
    void ResetDeltaTime();

    virtual bool notify(QObject *receiver, QEvent *e) override;

protected:
    AudioManager *m_audioManager         = nullptr;
    SceneManager *m_sceneManager         = nullptr;
    AssetsManager *m_assetsManager       = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    unsigned long long m_lastRenderTime = 0;

    // If its too low, Qt stops it when dragging smthing around the screen :(
    const int c_redrawDelay = 16; // 60 FPS cap

    virtual void OnMainLoopIterationEnd();

    friend class SceneManager;
    friend class BehaviourManager;
};

#endif // APPLICATION_H

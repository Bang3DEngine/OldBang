#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QApplication>
#include "Bang/ui_EditorWindow.h"
#include "Bang/WinUndef.h"

#include "Bang/IWindow.h"

/**
 *  This will be a singleton shared amongst ALL *.o and *.so
 *  Cross-library shared singleton
 */
class EditorState;
class QApplication;
class WindowEventManager;
class EditorPlayFlow;
class EditorWindow : public QObject,
                     public IWindow,
                     public Ui_EditorWindow
{
    Q_OBJECT

public:
    EditorWindow(QWidget *parent = nullptr);

    /**
     * @brief This must be called by main() in the mainBinary.
     * This method will register the EditorWindow singleton into the
     * SingletonManager, for later use in shared libraries.
     * @return The created Singleton,
     */
    static void InitFromMainBinary(QMainWindow *window, QApplication *app);
    bool IsSceneTabActive() const;
    bool IsGameTabActive() const;

    void RefreshDocksAndWindowTitles();

    static EditorWindow *GetInstance();
    Screen *GetScreen() const override;
    QMainWindow *GetMainWindow() const override;
    QApplication *GetApplication() const override;

private slots:
    void OnTabSceneGameChanged(int index);
    void Refresh();

private:
    static EditorWindow *s_win;

    QTimer m_refreshTimer;
    EditorState *m_editorState = nullptr;
    WindowEventManager *m_winEventManager = nullptr;
    EditorPlayFlow *m_playStopController = nullptr;

    friend class EditorState;
    friend class WindowEventManager;
    friend class EditorPlayFlow;
};

#endif // WINDOWMAIN_H

#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "Bang/Application.h"
#include "Bang/FileTracker.h"
#include "Bang/BehaviourRefresherTimer.h"

class EditorApplication : public Application
{
public:
    EditorApplication(int &argc, char **argv);
    virtual ~EditorApplication();

    virtual void InitManagers() override;
    static EditorApplication* GetInstance();

    virtual bool notify(QObject *receiver, QEvent *e) override;

protected:
    BehaviourRefresherTimer m_behaviourRefresherTimer;
    FileTracker *m_fileTracker = nullptr;

    virtual void OnMainLoopIterationEnd() override;

    friend class FileTracker;
};

#endif // EDITORAPPLICATION_H

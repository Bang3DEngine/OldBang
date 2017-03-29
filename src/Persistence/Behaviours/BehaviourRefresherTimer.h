#ifndef BEHAVIOURREFRESHERTHREAD_H
#define BEHAVIOURREFRESHERTHREAD_H

#include <QTimer>
#include <QObject>
#include "Bang/WinUndef.h"

class BehaviourRefresherTimer : public QObject
{
    Q_OBJECT

public:
    BehaviourRefresherTimer();

public:
    void RefreshBehaviours() const;

private slots:
    void OnRefreshTimer() const;

private:
    QTimer m_timer;
    int c_timeMs;
};

#endif // BEHAVIOURREFRESHERTHREAD_H

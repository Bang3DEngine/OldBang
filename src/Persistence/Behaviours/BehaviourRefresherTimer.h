#ifndef BEHAVIOURREFRESHERTHREAD_H
#define BEHAVIOURREFRESHERTHREAD_H

#include <QTimer>
#include <QObject>

class BehaviourRefresherTimer : public QObject
{
    Q_OBJECT

public:
    BehaviourRefresherTimer();

public:
    void RefreshBehavioursInScene() const;

private slots:
    void OnRefreshTimer() const;

private:
    QTimer m_timer;
    int c_timeMs = 10000;
};

#endif // BEHAVIOURREFRESHERTHREAD_H

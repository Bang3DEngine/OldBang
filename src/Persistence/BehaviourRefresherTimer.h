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
    void RefreshBehavioursInScene(bool synchronously) const;

private slots:
    void OnRefreshTimer() const;

private:
    QTimer m_timer;
};

#endif // BEHAVIOURREFRESHERTHREAD_H

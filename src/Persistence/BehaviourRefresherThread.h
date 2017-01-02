#ifndef BEHAVIOURREFRESHERTHREAD_H
#define BEHAVIOURREFRESHERTHREAD_H

#include <QThread>

class BehaviourRefresherThread :
        public QThread
{
public:
    BehaviourRefresherThread();
    void run() override;

private:
    const float c_RefreshDelaySeconds = 1.0f;
};

#endif // BEHAVIOURREFRESHERTHREAD_H

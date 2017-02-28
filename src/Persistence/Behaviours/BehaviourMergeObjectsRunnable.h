#ifndef BEHAVIOURMERGEOBJECTSRUNNABLE_H
#define BEHAVIOURMERGEOBJECTSRUNNABLE_H

#include <QObject>
#include <QRunnable>

class BehaviourMergeObjectsRunnable : public QObject,
                                      public QRunnable
{
    Q_OBJECT

public:
    BehaviourMergeObjectsRunnable();

protected:
    void run() override;

signals:
    void NotifySuccessMerging(QString mergedLibFilepath,
                              QString warnMessage);
    void NotifyFailedMerging(QString errorMessage);

private:
    void Merge();
};


#endif // BEHAVIOURMERGEOBJECTSRUNNABLE_H

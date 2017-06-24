#ifndef BEHAVIOURMERGEOBJECTSRUNNABLE_H
#define BEHAVIOURMERGEOBJECTSRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "Bang/WinUndef.h"

class BehaviourMergeObjectsRunnable : public QObject,
                                      public QRunnable
{
    Q_OBJECT

public:
    BehaviourMergeObjectsRunnable(bool forGame);

protected:
    void run() override;

signals:
    void NotifySuccessMerging(QString mergedLibFilepath,
                              bool mergingForGame,
                              QString warnMessage);
    void NotifyFailedMerging(QString errorMessage);

private:
    bool m_forGame = false;

    void Merge();
};


#endif // BEHAVIOURMERGEOBJECTSRUNNABLE_H

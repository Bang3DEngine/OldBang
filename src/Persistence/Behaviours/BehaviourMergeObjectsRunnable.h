#ifndef BEHAVIOURMERGEOBJECTSRUNNABLE_H
#define BEHAVIOURMERGEOBJECTSRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"

class BehaviourMergeObjectsRunnable : public QObject,
                                      public QRunnable
{
    Q_OBJECT

public:
    BehaviourMergeObjectsRunnable(bool forGame,
                                  const Path& outputDir);

protected:
    void run() override;

signals:
    void NotifySuccessMerging(QString mergedLibFilepath,
                              bool mergingForGame,
                              QString warnMessage);
    void NotifyFailedMerging(QString errorMessage);

private:
    bool m_forGame = false;
    Path m_outputDir;
};


#endif // BEHAVIOURMERGEOBJECTSRUNNABLE_H

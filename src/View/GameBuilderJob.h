#ifndef GAMEBUILDERTHREAD_H
#define GAMEBUILDERTHREAD_H

#include <QObject>

#include "Bang/String.h"

class GameBuilderJob : public QObject
{
    Q_OBJECT

    public:
        GameBuilderJob();
        void OnGameBuildingCanceled();

    public slots:
        void BuildGame();

    signals:
        void NotifyPercent(float);
        void NotifyMessage(const QString &message);
        void NotifyGameHasBeenBuilt();
        void NotifyGameBuildingHasFailed(const QString &errorMsg);

    protected:
        String m_executableFilepath = "";
        bool m_canceled = false;

    friend class GameBuilder;
};

#endif // GAMEBUILDERTHREAD_H

#ifndef GAMEBUILDERTHREAD_H
#define GAMEBUILDERTHREAD_H

#include <QThread>

#include "String.h"

class GameBuilderThread : public QThread
{
    Q_OBJECT
    public:
        GameBuilderThread();


    signals:
        void NotifyPercent(float);
        void NotifyMessage(String);

    protected:
        String m_executableFilepath = "";
        bool m_runGameAfterBuild = false;

        void BuildGame();
        void run() override;

    friend class GameBuilder;
};

#endif // GAMEBUILDERTHREAD_H

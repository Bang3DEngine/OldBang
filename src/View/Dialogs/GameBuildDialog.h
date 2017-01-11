#ifndef GAMEBUILDDIALOG_H
#define GAMEBUILDDIALOG_H

#include <QProgressDialog>

#include "String.h"

class GameBuildDialog : public QProgressDialog
{
    Q_OBJECT

public:
    GameBuildDialog();

public slots:
    /**
     * @brief SetPercent
     * @param percent In the range [0.0, 1.0]
     */
    void SetPercent(float percent);
    void SetMessage(const QString &message);
    void OnCanceled();
};

#endif // GAMEBUILDDIALOG_H

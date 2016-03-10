#ifndef EXPLORER_H
#define EXPLORER_H

#include <string>

#include <QListView>
#include <QMouseEvent>
#include <QFileSystemModel>

#include "Bang.h"
#include "Logger.h"

class Explorer : public QListView
{
    Q_OBJECT

private:
    QFileSystemModel *fileSystemModel = nullptr;

public:
    Explorer(QWidget *parent);

    void mouseDoubleClickEvent(QMouseEvent *e) override;
};

#endif // EXPLORER_H

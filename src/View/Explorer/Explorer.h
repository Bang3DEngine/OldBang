#ifndef EXPLORER_H
#define EXPLORER_H

#include <QListView>
#include <QFileSystemModel>

#include "Bang.h"
#include "Logger.h"

class Explorer : public QListView
{
    Q_OBJECT

private:
    QFileSystemModel *fileSystemModel = nullptr;
    QString rootPath = "";

public:
    Explorer(QWidget *parent);
};

#endif // EXPLORER_H

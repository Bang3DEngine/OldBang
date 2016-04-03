#ifndef EXPLORERDIRLIST_H
#define EXPLORERDIRLIST_H

#include <string>

#include <QTreeView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"
#include "Logger.h"
#include "Explorer.h"

//Directory list at the left of the Explorer itself
class ExplorerDirTree : public QTreeView
{
    Q_OBJECT

private:
    QFileSystemModel *fileSystemModel = nullptr;
    Explorer *explorer;

    std::string topPath;
    void setDir(const std::string &path);

public:
    ExplorerDirTree(QWidget *parent);
    virtual ~ExplorerDirTree();

public slots:
    void OnDirSelected(const QModelIndex & current, const QModelIndex & previous);
    void OnDirLoaded(QString dir);
};


#endif // EXPLORERDIRLIST_H

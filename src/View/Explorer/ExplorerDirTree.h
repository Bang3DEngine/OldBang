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
    QFileSystemModel *p_fileSystemModel = nullptr;
    Explorer *p_explorer;

    std::string m_topPath;
    void setDir(const std::string &path);

    void *p_lastSelectedModelIndexPointer = nullptr;

public:
    ExplorerDirTree(QWidget *parent);
    virtual ~ExplorerDirTree();

    QTimer *p_checkSelectionTimer;

public slots:
    void CheckSelection();
    void OnDirLoaded(QString dir);
};


#endif // EXPLORERDIRLIST_H

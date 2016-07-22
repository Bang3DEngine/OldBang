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
    QFileSystemModel *m_fileSystemModel = nullptr;
    Explorer *m_explorer;

    std::string m_topPath;
    void setDir(const std::string &path);

    void *m_lastSelectedModelIndexPointer = nullptr;

public:
    ExplorerDirTree(QWidget *parent);
    virtual ~ExplorerDirTree();

    QTimer *m_checkSelectionTimer;

public slots:
    void CheckSelection();
    void OnDirLoaded(QString dir);
};


#endif // EXPLORERDIRLIST_H

#ifndef EXPLORERDIRLIST_H
#define EXPLORERDIRLIST_H



#include <QTreeView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"
#include "Debug.h"
#include "Explorer.h"

//Directory list at the left of the Explorer itself
class ExplorerDirTree : public QTreeView
{
    Q_OBJECT

private:
    QFileSystemModel *m_fileSystemModel = nullptr;
    Explorer *m_explorer;

    String m_topPath;
    void setDir(const String &path);

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

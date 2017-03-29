#ifndef EXPLORERDIRLIST_H
#define EXPLORERDIRLIST_H

#include <QTimer>
#include <QTreeView>
#include "Bang/WinUndef.h"

#include "Bang/String.h"

class QTimer;
class Explorer;
class QFileSystemModel;
//Directory list at the left of the Explorer itself
class ExplorerDirTree : public QTreeView
{
    Q_OBJECT

public:
    ExplorerDirTree(QWidget *parent);
    virtual ~ExplorerDirTree();

    QTimer m_checkSelectionTimer;

public slots:
    void CheckSelection();
    void OnDirLoaded(QString dir);

private:
    QFileSystemModel *m_fileSystemModel = nullptr;
    Explorer *m_explorer = nullptr;

    String m_topPath = "";
    void SetDir(const String &path);

    void *m_lastSelectedModelIndexPointer = nullptr;
};


#endif // EXPLORERDIRLIST_H

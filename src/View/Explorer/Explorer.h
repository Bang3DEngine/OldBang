#ifndef EXPLORER_H
#define EXPLORER_H

#include <string>

#include <QListView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"

#include "Logger.h"

#include "Persistence.h"

#include "File.h"
#include "FileMesh.h"
#include "InspectorMeshFileWidget.h"
#include "InspectorImageFileWidget.h"

class Explorer : public QListView
{
    Q_OBJECT

    friend class ExplorerDirTree;

private:
    QFileSystemModel *fileSystemModel = nullptr;
    QToolButton *buttonDirUp;
    QToolButton *buttonChangeViewMode;

    void setDir(const std::string &path);
    std::string getCurrentDir() const;

    std::string lastSelectedFileName = "";
    QTimer *updateTimer = nullptr;

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    //Updates the Inspector with the selected file info
    void RefreshInspector();

public slots:
    void Refresh();

    void OnDirLoaded(QString dir);
    void OnButtonDirUpClicked();
    void OnButtonChangeViewModeClicked();
};

#endif // EXPLORER_H

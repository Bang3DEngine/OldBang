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
#include "MeshFile.h"
#include "ImageFile.h"
#include "MeshAssetFile.h"
#include "MeshAssetFileInspectable.h"
#include "MaterialAssetFile.h"
#include "MaterialAssetFileInspectable.h"
#include "Texture2DAssetFile.h"

#include "MeshFileInspectable.h"
#include "ImageFileInspectable.h"
#include "Texture2DAssetFileInspectable.h"

class Explorer : public QListView
{
    Q_OBJECT

    friend class ExplorerDirTree;

private:
    QFileSystemModel *m_fileSystemModel = nullptr;
    QToolButton *m_buttonDirUp = nullptr;
    QToolButton *m_buttonChangeViewMode = nullptr;
    IInspectable *lastIInspectableInInspector = nullptr;

    std::string m_lastSelectedFileName = "";
    QTimer *m_updateTimer = nullptr;

    void SetDir(const std::string &path);
    std::string GetCurrentDir() const;

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

#ifndef EXPLORER_H
#define EXPLORER_H

#include <string>

#include <QListView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"
#include "Logger.h"

class Explorer : public QListView
{
    Q_OBJECT

private:
    QFileSystemModel *fileSystemModel = nullptr;
    QToolButton *buttonDirUp;
    QToolButton *buttonChangeViewMode;

    static std::string topPath;
    void setDir(const std::string &path);
    std::string getCurrentDir() const;

public:
    Explorer(QWidget *parent);

    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    static std::string GetTopPath() { return topPath; }

public slots:
    void OnButtonDirUpClicked();
    void OnButtonChangeViewModeClicked();
};

#endif // EXPLORER_H

#ifndef FILEDIALOGASSET_H
#define FILEDIALOGASSET_H

class QEvent;

#include <string>
#include <QFileDialog>
#include <QString>

class FileDialogAsset : public QFileDialog
{
    Q_OBJECT

public:
    explicit FileDialogAsset(const std::string& topDir,
                             const std::string& fileExtension,
                             QWidget *parent = nullptr);

    QString topDir;
    std::string fileExtension;

    bool eventFilter(QObject *o, QEvent *e);
    void SetTopDir(const std::string &topDirectory);
    QString GetTopDir() const;

    std::string GetOpenFileName();

private:
    bool PathFits(const QString &path) const;

private slots:
    void CheckHistory();
    void CheckGoToParent();
    void CheckLineEdit(const QString &text);
};

#endif // FILEDIALOGASSET_H

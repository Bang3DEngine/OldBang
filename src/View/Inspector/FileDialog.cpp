#include "FileDialog.h"

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QList>
#include <QToolButton>
#include <QDir>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QEvent>
#include <QKeyEvent>
#include <QAbstractButton>
#include <QCompleter>
#include <QAbstractItemView>
#include <QFileInfo>

#include "Logger.h"

FileDialog::FileDialog(const std::string& topDir,
                       const std::string& fileExtension,
                       QWidget *parent) : QFileDialog(parent)
{
    connect(this, SIGNAL(directoryEntered(QString)), this, SLOT(CheckHistory()));
    connect(this, SIGNAL(directoryEntered(QString)), this, SLOT(CheckGoToParent()));

    connect(findChild<QToolButton *>("backButton"), SIGNAL(clicked()), this, SLOT(CheckGoToParent()));
    connect(findChild<QToolButton *>("forwardButton"), SIGNAL(clicked()), this, SLOT(CheckGoToParent()));
    connect(findChild<QLineEdit *>("fileNameEdit"), SIGNAL(textChanged(QString)), this, SLOT(CheckLineEdit(QString)));

    findChild<QLineEdit *>("fileNameEdit")->installEventFilter(this);
    findChild<QWidget *>("listView")->installEventFilter(this);
    findChild<QWidget *>("treeView")->installEventFilter(this);
    findChild<QLineEdit *>("fileNameEdit")->completer()->popup()->installEventFilter(this);
    setOption(DontUseNativeDialog, true);

    this->fileExtension = fileExtension;
    SetTopDir(topDir);
}

bool FileDialog::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() != QEvent::KeyPress) return false;

    int key = static_cast<QKeyEvent *>(e)->key();
    if (o->objectName() == "listView" || o->objectName() == "treeView")
    {
        return (Qt::Key_Backspace == key && !PathFits(directory().absolutePath()));
    }
    else
    {
        if (Qt::Key_Return != key && Qt::Key_Enter != key)
            return false;
        QString text = findChild<QLineEdit *>("fileNameEdit")->text();
        QString path = QDir::cleanPath(directory().absolutePath() + (text.startsWith("/") ? "" : "/") + text);
        bool a = QDir(text).isAbsolute();
        return !((!a && PathFits(path)) || (a && PathFits(text)));
    }
}

void FileDialog::SetTopDir(const std::string &topDirectory)
{
    QString path = QString::fromStdString(topDirectory);
    if (path == topDir) return;

    topDir = (!path.isEmpty() && QFileInfo(path).isDir()) ? path : QString();
    if (!PathFits(path))
    {
        setDirectory(topDir);
        CheckHistory();
        CheckLineEdit(findChild<QLineEdit *>("fileNameEdit")->text());
    }
    else
    {
        QLineEdit *ledt = findChild<QLineEdit *>("fileNameEdit");
        ledt->setText(ledt->text());
    }
    findChild<QWidget *>("lookInCombo")->setEnabled(topDir.isEmpty());
    findChild<QWidget *>("sidebar")->setEnabled(topDir.isEmpty());
    CheckGoToParent();
}

QString FileDialog::GetTopDir() const
{
    return topDir;
}

std::string FileDialog::GetOpenFileName()
{
    QString filter = QString::fromStdString(fileExtension + "(*." + fileExtension + ")");
    return this->getOpenFileName(this->parentWidget(),
                                 tr("Select an asset"),
                                 topDir,
                                 filter
                                 ).toStdString();
}

bool FileDialog::PathFits(const QString &path) const
{
    return topDir.isEmpty() || (path.startsWith(topDir) && path.length() > topDir.length());
}

void FileDialog::CheckHistory()
{
    QStringList list = history();
    for (int i = list.size() - 1; i >= 0; --i)
        if (!PathFits(list.at(i)))
            list.removeAt(i);
    setHistory(list);
}

void FileDialog::CheckGoToParent()
{
    findChild<QToolButton *>("toParentButton")->setEnabled(PathFits(directory().absolutePath()));
}

void FileDialog::CheckLineEdit(const QString &text)
{
    QAbstractButton *btn = findChild<QDialogButtonBox *>("buttonBox")->buttons().first();
    QString path = QDir::cleanPath(directory().absolutePath() + (text.startsWith("/") ? "" : "/") + text);
    bool a = QDir(text).isAbsolute();
    btn->setEnabled(btn->isEnabled() && ((!a && PathFits(path)) || (a && PathFits(text))));
}

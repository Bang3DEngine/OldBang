#ifndef INSPECTORASSETSW_H
#define INSPECTORASSETSW_H

#include <QDir>
#include <QLineEdit>

#include "Bang/String.h"
#include "Bang/AttributeWidget.h"
#include "Bang/IWindowEventManagerListener.h"

class QHBoxLayout;
class FileLineEdit;
class AttrWidgetFile : public AttributeWidget,
                       public IWindowEventManagerListener
{
    Q_OBJECT

public:
    String m_filepath = "";
    String m_fileExtension = "";
    FileLineEdit *m_filepathLineEdit = nullptr;

public:
    AttrWidgetFile(const XMLAttribute &xmlAttribute,
                   InspectorWidget *m_inspectorWidget);
    virtual ~AttrWidgetFile();

    virtual void SetValue(const String &filepath, bool draggedFile = false);
    virtual String GetValue();

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;
    virtual void Refresh(const XMLAttribute &attribute) override;

public slots:
    void OnDoubleClick();
    void Browse();

private:
    QLabel *m_iconLabel = nullptr;
    QHBoxLayout *m_hLayout = nullptr;

    void RefreshIcon();
};

////////////////////////////////////////////////////

class FileLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    FileLineEdit(QWidget *parent = nullptr);

    void SetBold(bool bold);
    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;

public slots:
    void Deselect();

signals:
    void DoubleClicked();
};

#endif // INSPECTORASSETSW_H

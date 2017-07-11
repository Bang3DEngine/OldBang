#ifndef INSPECTORASSETSW_H
#define INSPECTORASSETSW_H

#include <QDir>
#include <QLineEdit>
#include "Bang/WinUndef.h"

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
    AttrWidgetFile(const XMLAttribute &xmlAttribute);
    virtual ~AttrWidgetFile();

    virtual void SetValue(const Path &filepath, bool draggedFile = false);
    virtual String GetValue() const;

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;
    virtual void Refresh(const XMLAttribute &attribute) override;
    virtual XMLAttribute GetXMLAttribute() const override;

    const Path& GetPath() const;

public slots:
    void OnDoubleClick();
    void Browse();

private:
    QLabel *m_iconLabel = nullptr;
    QHBoxLayout *m_hLayout = nullptr;
    String m_attrName = "";

    Path m_filepath;
    String m_allowedExtensions;
    FileLineEdit *m_filepathLineEdit = nullptr;

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

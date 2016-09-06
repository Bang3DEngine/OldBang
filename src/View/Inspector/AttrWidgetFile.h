#ifndef INSPECTORASSETSW_H
#define INSPECTORASSETSW_H

#include "Array.h"

#include <QFileDialog>
#include <QDir>

#include "Asset.h"
#include "Mesh.h"
#include "Explorer.h"
#include "FileDialog.h"
#include "AttributeWidget.h"
#include "WindowEventManager.h"

class FileLineEdit;
class AttrWidgetFile : public AttributeWidget,
                       public IWindowEventManagerListener
{
    Q_OBJECT

private:
public:
    String m_fileExtension = "";
    String m_filepath = "";
    FileLineEdit *m_filepathLineEdit = nullptr;

public:
    AttrWidgetFile(const XMLAttribute &xmlAttribute,
                   InspectorWidget *m_inspectorWidget);
    virtual ~AttrWidgetFile();

    virtual void SetValue(const String &filepath, bool draggedFile = false);
    virtual String GetValue();

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDropHere(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;
    virtual void Refresh(const XMLAttribute &attribute) override;

public slots:
    void OnDoubleClick();
    void Browse();
};

////////////////////////////////////////////////////

class FileLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    FileLineEdit(QWidget *parent = nullptr);

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;

public slots:
    void Deselect();

signals:
    void DoubleClicked();
};

#endif // INSPECTORASSETSW_H

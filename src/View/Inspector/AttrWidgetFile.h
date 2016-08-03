#ifndef INSPECTORASSETSW_H
#define INSPECTORASSETSW_H

#include <vector>
#include <string>
#include <QFileDialog>
#include <QDir>

#include "Asset.h"
#include "Mesh.h"
#include "Explorer.h"
#include "FileDialog.h"
#include "AttributeWidget.h"
#include "WindowEventManager.h"

class FileLineEdit;
class AttrWidgetFile : public AttributeWidget
{
    Q_OBJECT

private:
public:
    std::string m_fileExtension = "";
    std::string m_filepath = "";
    FileLineEdit *m_filepathLineEdit = nullptr;

public:
    AttrWidgetFile(const std::string &labelString,
                   const std::string &fileExtension,
                   bool readonly,
                   InspectorWidget *m_parent);

    virtual void SetValue(const std::string &filepath);
    virtual std::string GetValue();

    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;

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

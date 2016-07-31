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

class AttrWidgetFile : public AttributeWidget
{
    Q_OBJECT

private:
public:
    std::string m_fileExtension = "";
    std::string m_filepath = "";
    QLineEdit *m_filepathLineEdit = nullptr;

public:
    AttrWidgetFile(const std::string &labelString,
                   const std::string &fileExtension,
                   InspectorWidget *m_parent);

    virtual void SetValue(const std::string &filepath);
    virtual std::string GetValue();

public slots:
    void Browse();
};

#endif // INSPECTORASSETSW_H

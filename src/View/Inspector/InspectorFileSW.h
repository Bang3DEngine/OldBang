#ifndef INSPECTORPARTASSETSW_H
#define INSPECTORPARTASSETSW_H

#include <vector>
#include <string>
#include <QFileDialog>
#include <QDir>

#include "Asset.h"
#include "Mesh.h"
#include "Explorer.h"
#include "FileDialog.h"
#include "InspectorSW.h"
#include "WindowEventManager.h"

class InspectorFileSW : public InspectorSW
{
    Q_OBJECT

private:
    std::string fileExtension;
    std::string assetPath;
    QLineEdit *filepathLineEdit;

public:
    InspectorFileSW(const std::string &labelString,
                         const std::string &assetPath,
                         const std::string &fileExtension,
                         InspectorWidget *parent);

    virtual void SetValue(const std::string &assetPath);
    virtual std::string GetValue();

public slots:
    void Browse();
};

#endif // INSPECTORPARTASSETSW_H

#ifndef MODELINSPECTORWIDGET_H
#define MODELINSPECTORWIDGET_H

#include "Bang/ModelFile.h"
#include "Bang/FileInspectorWidget.h"

class ModelInspectorWidget : public FileInspectorWidget
{
public:
    ModelInspectorWidget(const ModelFile &modelFile);
    virtual ~ModelInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
};


#endif // MODELINSPECTORWIDGET_H

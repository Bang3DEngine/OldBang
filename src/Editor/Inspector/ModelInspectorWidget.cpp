#include "Bang/ModelInspectorWidget.h"

ModelInspectorWidget::ModelInspectorWidget(const ModelFile &modelFile)
    : FileInspectorWidget(modelFile)
{
}

ModelInspectorWidget::~ModelInspectorWidget()
{

}
void ModelInspectorWidget::OnDestroy()
{
    FileInspectorWidget::OnDestroy();
}

void ModelInspectorWidget::InitExtra()
{
    FileInspectorWidget::InitExtra();
}

void ModelInspectorWidget::OnUpdate()
{
    FileInspectorWidget::OnUpdate();
}

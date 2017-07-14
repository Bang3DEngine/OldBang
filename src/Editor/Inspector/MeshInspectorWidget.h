#ifndef MESHINSPECTORWIDGET_H
#define MESHINSPECTORWIDGET_H

#include "Bang/Mesh.h"
#include "Bang/MeshFile.h"
#include "Bang/AssetInspectorWidget.h"

class MeshInspectorWidget : public AssetInspectorWidget<Mesh>
{
public:
    MeshInspectorWidget(const MeshFile &meshFile);
    virtual ~MeshInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;

    AttrWidgetFile *m_modelFileAW = nullptr;
};


#endif // MESHINSPECTORWIDGET_H

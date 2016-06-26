#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include <vector>

#include "VBO.h"
#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "FileReader.h"
#include "ShaderContract.h"

#include "LineRenderer.h"

class SingleLineRenderer : public LineRenderer
{

public:
    SingleLineRenderer();
    virtual ~SingleLineRenderer();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;
    virtual ICloneable *Clone() const override;

    void SetOrigin(Vector3 o);
    void SetDestiny(Vector3 d);

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
    #endif
};

#endif // SINGLELINERENDERER_H

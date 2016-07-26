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

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void SetOrigin(const Vector3 &o);
    void SetDestiny(const Vector3 &d);

    Vector3 GetOrigin() const;
    Vector3 GetDestiny() const;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded() override;
    virtual void OnInspectorInfoChanged(InspectorWidget *source) override;
    #endif

    void WriteInternal(std::ostream &f) const;
    void ReadInternal(std::istream &f);
};

#endif // SINGLELINERENDERER_H

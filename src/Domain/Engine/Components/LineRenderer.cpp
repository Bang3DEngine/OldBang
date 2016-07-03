#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
#ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo("Material",
                    Material::GetFileExtensionStatic()),
        new InspectorVFloatSWInfo("Line Width", {0.0f})
    });
#endif

    vbo = new VBO();
    vao = new VAO();

    Material *m = AssetsManager::GetAsset<Material>(
                "./Assets/Engine/Materials/lines.bmat" );
    SetMaterial(m);
    SetLineWidth(1.0f);
}

LineRenderer::~LineRenderer()
{

}

const std::string LineRenderer::ToString() const
{
    return "LineRenderer";
}

std::string LineRenderer::GetName() const { return "LineRenderer"; }


void LineRenderer::BindPointsToVAO() const
{
    if(points.size() >= 2 &&
       material  && material->GetShaderProgram() )
    {
        vbo->Fill(points.data(), points.size() * sizeof(Vector3));
        GLint verticesShaderLocation = material->GetShaderProgram()->
                GetLocation(ShaderContract::Vertex_In_Position_Raw);
        vao->UnBindVBO(verticesShaderLocation);
        vao->BindVBO(vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void LineRenderer::ActivateStatesBeforeRendering() const
{
    Renderer::ActivateStatesBeforeRendering();
}

void LineRenderer::RenderWithoutBindingMaterial() const
{
    ActivateStatesBeforeRendering();

    Matrix4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    vao->Bind();
    glDrawArrays(drawLinesMode, 0, points.size());
    vao->UnBind();
}

void LineRenderer::OnRender()
{
    Scene *scene = GetOwner()->GetScene();
    Camera *cam = scene->GetCamera();
    if(!CAN_USE_COMPONENT(cam))
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << scene << " does not have a set Camera.");
    }

    NONULL(material); NONULL(material->GetShaderProgram());

    Render();
}

void LineRenderer::Render() const
{
    material->Bind();
    RenderWithoutBindingMaterial();
    material->UnBind();
}

void LineRenderer::SetMaterial(Material *m)
{
    material = m;
    BindPointsToVAO();
}

Box LineRenderer::GetBoundingBox() const
{
    Vector3 minp(999999.9f);
    Vector3 maxp(-999999.9f);
    for(int i = 0; i < points.size(); ++i)
    {
        Vector3 p = points[i];
        if(p.x < minp.x) minp.x = p.x;
        if(p.y < minp.y) minp.y = p.y;
        if(p.z < minp.z) minp.z = p.z;
        if(p.x > minp.x) maxp.x = p.x;
        if(p.y > minp.y) maxp.y = p.y;
        if(p.z > minp.z) maxp.z = p.z;
    }

    return Box(minp, maxp);
}

const std::vector<Vector3> &LineRenderer::GetPoints() const
{
    return points;
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* LineRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo =
            static_cast<InspectorFileSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(0));

    if (material )
    {
        if(material->GetFilepath() != "")
        {
            matInfo->filepath = material->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(matInfo->filepath, (void*)material);
        }
    }
    else
    {
        matInfo->filepath = "-";
    }

    InspectorVFloatSWInfo *widthInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(1));
    widthInfo->value = {GetLineWidth()};

    return &inspectorComponentInfo;
}

void LineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    if(materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    float width = source->GetSWVectorFloatValue("Line Width")[0];
    SetLineWidth(width);
}

#endif

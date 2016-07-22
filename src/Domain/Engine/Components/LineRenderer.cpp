#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
#ifdef BANG_EDITOR
    m_inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo("Material",
                    Material::GetFileExtensionStatic()),
        new InspectorVFloatSWInfo("Line Width", {0.0f})
    });
#endif

    m_vbo = new VBO();
    m_vao = new VAO();

    Material *m = AssetsManager::GetAsset<Material>(
                "./Assets/Engine/Materials/Line.bmat" );
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
    if (m_points.size() >= 2 &&
       m_material  && m_material->GetShaderProgram() )
    {
        m_vbo->Fill(m_points.data(), m_points.size() * sizeof(Vector3));
        GLint verticesShaderLocation = m_material->GetShaderProgram()->
                GetAttribLocation(ShaderContract::Attr_Vertex_In_Position_Raw);
        m_vao->UnBindVBO(verticesShaderLocation);
        m_vao->BindVBO(m_vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void LineRenderer::RenderWithoutBindingMaterial() const
{
    Matrix4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    m_vao->Bind();
    glDrawArrays(m_drawLinesMode, 0, m_points.size());
    m_vao->UnBind();
}

void LineRenderer::SetMaterial(Material *m)
{
    m_material = m;
    BindPointsToVAO();
}

Box LineRenderer::GetBoundingBox() const
{
    Vector3 minp(999999.9f);
    Vector3 maxp(-999999.9f);
    for (int i = 0; i < m_points.size(); ++i)
    {
        Vector3 p = m_points[i];
        if (p.x < minp.x) minp.x = p.x;
        if (p.y < minp.y) minp.y = p.y;
        if (p.z < minp.z) minp.z = p.z;
        if (p.x > minp.x) maxp.x = p.x;
        if (p.y > minp.y) maxp.y = p.y;
        if (p.z > minp.z) maxp.z = p.z;
    }

    return Box(minp, maxp);
}

const std::vector<Vector3> &LineRenderer::GetPoints() const
{
    return m_points;
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* LineRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo =
            static_cast<InspectorFileSWInfo*>(
                m_inspectorComponentInfo.GetSlotInfo(0));

    if (m_material )
    {
        if (m_material->GetFilepath() != "")
        {
            matInfo->filepath = m_material->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(matInfo->filepath, (void*)m_material);
        }
    }
    else
    {
        matInfo->filepath = "-";
    }

    InspectorVFloatSWInfo *widthInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                m_inspectorComponentInfo.GetSlotInfo(1));
    widthInfo->m_value = {GetLineWidth()};

    return &m_inspectorComponentInfo;
}

void LineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    if (materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    float width = source->GetSWVectorFloatValue("Line Width")[0];
    SetLineWidth(width);
}

#endif

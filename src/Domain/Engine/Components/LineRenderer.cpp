#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
    m_vbo = new VBO();
    m_vao = new VAO();
}

LineRenderer::~LineRenderer()
{

}

const String LineRenderer::ToString() const
{
    return "LineRenderer";
}

String LineRenderer::GetName() const { return "LineRenderer"; }


void LineRenderer::BindPointsToVAO() const
{
    if (m_points.Size() >= 2 &&
       m_material  && m_material->GetShaderProgram() )
    {
        m_vbo->Fill(m_points.Data(), m_points.Size() * sizeof(Vector3));
        GLint verticesShaderLocation = m_material->GetShaderProgram()->
                GetAttribLocation(ShaderContract::Attr_Vertex_In_Position_Raw);
        m_vao->UnBindVBO(verticesShaderLocation);
        m_vao->BindVBO(m_vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void LineRenderer::RenderWithoutBindingMaterial() const
{
    m_vao->Bind();
    glDrawArrays(GLint(m_drawLinesMode), 0, m_points.Size());
    m_vao->UnBind();
}

void LineRenderer::SetMaterial(Material *m)
{
    m_material = m;
    BindPointsToVAO();
}

Box LineRenderer::GetBoundingBox() const
{
    if (m_points.Empty()) { return Box::Empty; }

    Vector3 minp = m_points.Front();
    Vector3 maxp = m_points.Front();
    for (const Vector3 &p : m_points)
    {
        minp.x = glm::min(minp.x, p.x);
        minp.y = glm::min(minp.y, p.y);
        minp.z = glm::min(minp.z, p.z);
        maxp.x = glm::max(maxp.x, p.x);
        maxp.y = glm::max(maxp.y, p.y);
        maxp.z = glm::max(maxp.z, p.z);
    }

    // Add a bit in every dimensions, to avoid flattened Rects
    const float strokeAdd = 0.01f;
    minp.x -= strokeAdd; maxp.x += strokeAdd;
    minp.y -= strokeAdd; maxp.y += strokeAdd;
    minp.z -= strokeAdd; maxp.z += strokeAdd;
    return Box(minp, maxp);
}

const Array<Vector3> &LineRenderer::GetPoints() const
{
    return m_points;
}

#ifdef BANG_EDITOR
void LineRenderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void LineRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void LineRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Renderer::ReadXMLInfo(xmlInfo);
}

void LineRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Renderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("LineRenderer");
}

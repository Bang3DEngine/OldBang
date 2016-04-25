#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
    #ifdef BANG_EDITOR
        inspectorPartInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material", Material::GetFileExtension()),
            new InspectorVFloatSWInfo("Origin", {0.0f, 0.0f, 0.0f}),
            new InspectorVFloatSWInfo("Destiny", {0.0f, 0.0f, 0.0f}),
            new InspectorVFloatSWInfo("Line Width", {0.0f})
        });
    #endif

    points.resize(2);

    vbo = new VBO();
    vbo->Fill(points.data(), points.size() * sizeof(glm::vec3));

    vao = new VAO();
    vao->BindVBO(vbo, 0, 3 * points.size(), GL_FLOAT);

    glEnable(GL_LINE_SMOOTH); //Line antialiasing
}

LineRenderer::~LineRenderer()
{

}

void LineRenderer::BindPointsToVAO() const
{
    if(material != nullptr && material->GetShaderProgram() != nullptr)
    {
        vbo->Fill(points.data(), points.size() * sizeof(glm::vec3));
        GLint verticesShaderLocation = material->GetShaderProgram()->
                GetLocation(ShaderContract::Vertex_In_Position_Raw);
        vao->UnBindVBO(verticesShaderLocation);
        vao->BindVBO(vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void LineRenderer::_OnRender()
{
    Stage *stage = GetOwner()->GetStage();
    Camera *cam = stage->GetCamera();
    if(CAN_USE_PART(cam))
    {
        Render();
    }
    else
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << stage << " does not have a set Camera.");
    }
}

void LineRenderer::Render() const
{
    if(material == nullptr)
    {
        Logger_Verbose(owner << " could not be rendered because it does not have a Material (or it's disabled')");
        return;
    }
    else if(material->GetShaderProgram() == nullptr)
    {
        Logger_Error(owner << " has a Material with no ShaderProgram. Can't render.");
        return;
    }

    glm::mat4 view(1.0f), projection(1.0f);
    Camera *camera = owner->GetStage()->GetCamera();
    if( CAN_USE_PART(camera) )
    {
        camera->GetViewMatrix(view);
        camera->GetProjectionMatrix(projection);
    }
    glm::mat4 pvm = projection * view;

    vao->Bind();
    material->Bind();
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_Model, glm::mat4(1.0f), false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_View, view, false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection, projection, false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM, pvm, false);

    glLineWidth(width);
    glDrawArrays(Mesh::RenderMode::Lines, 0, points.size());

    material->UnBind();
    vao->UnBind();
}

void LineRenderer::SetMaterial(Material *m)
{
    material = m;
    BindPointsToVAO();
}

void LineRenderer::SetOrigin(glm::vec3 o)
{
    points[0] = o;
    BindPointsToVAO();
}

void LineRenderer::SetDestiny(glm::vec3 d)
{
    points[1] = d;
    BindPointsToVAO();
}

float LineRenderer::GetLineWidth() const
{
    return width;
}

void LineRenderer::SetLineWidth(float w)
{
    width = w;
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* LineRenderer::GetPartInfo()
{
    InspectorFileSWInfo* matInfo =
            static_cast<InspectorFileSWInfo*>(inspectorPartInfo.GetSlotInfo(0));

    if (material != nullptr)
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

    InspectorVFloatSWInfo *originInfo  =
            static_cast<InspectorVFloatSWInfo*>(inspectorPartInfo.GetSlotInfo(1));
    originInfo->value = {points[0].x, points[0].y, points[0].z};

    InspectorVFloatSWInfo *destinyInfo  =
            static_cast<InspectorVFloatSWInfo*>(inspectorPartInfo.GetSlotInfo(2));
    destinyInfo->value = {points[1].x, points[1].y, points[1].z};

    InspectorVFloatSWInfo *widthInfo  =
            static_cast<InspectorVFloatSWInfo*>(inspectorPartInfo.GetSlotInfo(3));
    widthInfo->value = {GetLineWidth()};

    return &inspectorPartInfo;
}

void LineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    if(materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    std::vector<float> origin = source->GetSWVectorFloatValue("Origin");
    points[0] = glm::vec3(origin[0], origin[1], origin[2]);

    std::vector<float> destiny = source->GetSWVectorFloatValue("Destiny");
    points[1] = glm::vec3(destiny[0], destiny[1], destiny[2]);

    std::vector<float> widthInfo = source->GetSWVectorFloatValue("Line Width");
    SetLineWidth(widthInfo[0]);
}


void LineRenderer::Write(std::ostream &f) const
{
    f << "<LineRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(material->GetFilepath(), f);
    FileWriter::Write(glm::vec3(points[0].x, points[0].y, points[0].z), f);
    FileWriter::Write(glm::vec3(points[1].x, points[1].y, points[1].z), f);
    FileWriter::Write(GetLineWidth(), f);
    f << "</LineRenderer>" << std::endl;
}

void LineRenderer::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this);
    SetMaterial( AssetsManager::GetAsset<Material>( FileReader::ReadString(f) ) );
    SetOrigin(FileReader::ReadVec3(f));
    SetDestiny(FileReader::ReadVec3(f));
    SetLineWidth(FileReader::ReadFloat(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif

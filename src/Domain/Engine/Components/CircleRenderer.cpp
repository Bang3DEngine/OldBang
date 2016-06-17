#include "CircleRenderer.h"

CircleRenderer::CircleRenderer()
{
    #ifdef BANG_EDITOR
        inspectorComponentInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material",
                        Material::GetFileExtensionStatic()),
            new InspectorVFloatSWInfo("Radius", {1.0f}),
            new InspectorVFloatSWInfo("Segments", {32.0f}),
            new InspectorVFloatSWInfo("Line Width", {1.0f})
        });
    #endif

    vbo = new VBO();
    vao = new VAO();
    BindPointsToVAO();

    SetLineWidth(2.0f);

    Material *m = AssetsManager::GetAsset<Material>(
                "./res/Materials/lines.bmat" );
    SetMaterial(m);
}

CircleRenderer::~CircleRenderer()
{

}

const std::string CircleRenderer::ToString() const
{
    return "CircleRenderer";
}

std::string CircleRenderer::GetName() const { return "CircleRenderer"; }


void CircleRenderer::BindPointsToVAO() const
{
    if(material != nullptr && material->GetShaderProgram() != nullptr)
    {
        std::vector<Vector3> vertices;
        GenerateVertices(&vertices);

        vbo->Fill(vertices.data(), vertices.size() * sizeof(Vector3));
        GLint verticesShaderLocation = material->GetShaderProgram()->
                GetLocation(ShaderContract::Vertex_In_Position_Raw);
        vao->UnBindVBO(verticesShaderLocation);
        vao->BindVBO(vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void CircleRenderer::GenerateVertices(std::vector<Vector3> *verts) const
{
    std::vector<Vector3> &vertices = *verts;
    vertices.clear();
    vertices.resize(segments + 1);

    float step = (2.0f * 3.141592f) / segments;
    for(int i = 0;  i < segments + 1; ++i)
    {
        vertices[i] = Vector3(glm::cos(step*i), glm::sin(step*i), 0.0f) * radius;
    }
}

void CircleRenderer::ActivateStatesBeforeRendering() const
{
    Renderer::ActivateStatesBeforeRendering();
}

void CircleRenderer::RenderWithoutBindingMaterial() const
{
    ActivateStatesBeforeRendering();

    Matrix4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    vao->Bind();
    glDrawArrays(Renderer::RenderMode::LineStrip, 0, segments + 1);
    vao->UnBind();
}

void CircleRenderer::OnRender()
{
    Scene *scene = GetOwner()->GetScene();
    Camera *cam = scene->GetCamera();
    if(!CAN_USE_COMPONENT(cam))
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << scene << " does not have a set Camera.");
    }

    if(material == nullptr)
    {
        Logger_Verbose(owner << " could not be rendered because it does " <<
                       "not have a Material (or it's disabled')");
        return;
    }
    else if(material->GetShaderProgram() == nullptr)
    {
        Logger_Error(owner << " has a Material with no ShaderProgram." <<
                     "Can't render.");
        return;
    }

    Render();
}

void CircleRenderer::Render() const
{
    material->Bind();
    RenderWithoutBindingMaterial();
    material->UnBind();
}

void CircleRenderer::SetMaterial(Material *m)
{
    material = m;
    BindPointsToVAO();
}

Box CircleRenderer::GetBoundingBox() const
{
    return Box(Vector3(-radius, -radius, 0.0001f),
               Vector3(radius, radius, 0.0001f));
}

void CircleRenderer::SetRadius(float radius)
{
    this->radius = radius;
    BindPointsToVAO();
}

void CircleRenderer::SetSegments(int segments)
{
    this->segments = segments;
    BindPointsToVAO();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* CircleRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo =
            static_cast<InspectorFileSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(0));

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

    InspectorVFloatSWInfo *radiusInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(1));
    radiusInfo->value = {radius};

    InspectorVFloatSWInfo *segmentsInfo =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(2));
    segmentsInfo->value = {segments};

    InspectorVFloatSWInfo *widthInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(3));
    widthInfo->value = {GetLineWidth()};

    return &inspectorComponentInfo;
}

void CircleRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    if(materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    SetRadius(source->GetSWVectorFloatValue("Radius")[0]);
    SetSegments(int(source->GetSWVectorFloatValue("Segments")[0]));

    float width = source->GetSWVectorFloatValue("Line Width")[0];
    SetLineWidth(width);
}


void CircleRenderer::Write(std::ostream &f) const
{
    f << "<CircleRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(material->GetFilepath(), f);
    FileWriter::Write(radius, f);
    FileWriter::Write(segments, f);
    FileWriter::Write(GetLineWidth(), f);
    f << "</CircleRenderer>" << std::endl;
}

void CircleRenderer::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetMaterial( AssetsManager::GetAsset<Material>(FileReader::ReadString(f)));
    SetRadius(FileReader::ReadFloat(f));
    SetSegments(FileReader::ReadFloat(f));
    SetLineWidth(FileReader::ReadFloat(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif

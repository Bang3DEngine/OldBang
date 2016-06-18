#include "CircleRenderer.h"

CircleRenderer::CircleRenderer()
{
    #ifdef BANG_EDITOR
        inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Radius", {1.0f}));
        inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Segments", {32.0f}));
    #endif

    this->drawLinesMode = RenderMode::LineStrip;
}

CircleRenderer::~CircleRenderer()
{

}

const std::string CircleRenderer::ToString() const
{
    return "CircleRenderer";
}

std::string CircleRenderer::GetName() const { return "CircleRenderer"; }

void CircleRenderer::GeneratePoints()
{
    points.clear();
    points.resize(segments + 1);

    float step = (2.0f * 3.141592f) / segments;
    for(int i = 0;  i < segments + 1; ++i)
    {
        points[i] = Vector3(glm::cos(step*i), glm::sin(step*i), 0.0f) * radius;
    }
}

void CircleRenderer::SetRadius(float radius)
{
    this->radius = radius;
    GeneratePoints();
    BindPointsToVAO();
}

void CircleRenderer::SetSegments(int segments)
{
    this->segments = segments;
    GeneratePoints();
    BindPointsToVAO();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* CircleRenderer::GetComponentInfo()
{
    LineRenderer::GetComponentInfo();

    InspectorVFloatSWInfo *radiusInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(2));
    radiusInfo->value = {radius};

    InspectorVFloatSWInfo *segmentsInfo =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(3));
    segmentsInfo->value = {float(segments)};

    return &inspectorComponentInfo;
}

void CircleRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    LineRenderer::OnSlotValueChanged(source);

    SetRadius(source->GetSWVectorFloatValue("Radius")[0]);
    SetSegments(int(source->GetSWVectorFloatValue("Segments")[0]));
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

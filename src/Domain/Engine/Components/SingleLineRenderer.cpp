#include "SingleLineRenderer.h"

SingleLineRenderer::SingleLineRenderer()
{
    #ifdef BANG_EDITOR
        inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Origin", {0.0f, 0.0f, 0.0f}));
        inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Destiny", {0.0f, 0.0f, 0.0f}));
    #endif

    points.resize(2);
    points[0] = Vector3(0.0f);
    points[1] = Vector3(1.0f);

    this->drawLinesMode = RenderMode::Lines;
}

SingleLineRenderer::~SingleLineRenderer()
{

}

const std::string SingleLineRenderer::ToString() const
{
    return "SingleLineRenderer";
}

std::string SingleLineRenderer::GetName() const { return "SingleLineRenderer"; }

ICloneable *SingleLineRenderer::Clone() const
{
    return new SingleLineRenderer();
}

void SingleLineRenderer::SetOrigin(Vector3 o)
{
    points[0] = o;
    BindPointsToVAO();
}

void SingleLineRenderer::SetDestiny(Vector3 d)
{
    points[1] = d;
    BindPointsToVAO();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* SingleLineRenderer::GetComponentInfo()
{
    LineRenderer::GetComponentInfo();

    InspectorVFloatSWInfo *originInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(2));
    originInfo->value = {points[0].x, points[0].y, points[0].z};

    InspectorVFloatSWInfo *destinyInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(3));
    destinyInfo->value = {points[1].x, points[1].y, points[1].z};

    return &inspectorComponentInfo;
}

void SingleLineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    LineRenderer::OnSlotValueChanged(source);

    std::vector<float> origin = source->GetSWVectorFloatValue("Origin");
    points[0] = Vector3(origin[0], origin[1], origin[2]);

    std::vector<float> destiny = source->GetSWVectorFloatValue("Destiny");
    points[1] = Vector3(destiny[0], destiny[1], destiny[2]);
}


void SingleLineRenderer::Write(std::ostream &f) const
{
    f << "<SingleLineRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(material->GetFilepath(), f);
    FileWriter::Write(Vector3(points[0].x, points[0].y, points[0].z), f);
    FileWriter::Write(Vector3(points[1].x, points[1].y, points[1].z), f);
    FileWriter::Write(GetLineWidth(), f);
    f << "</SingleLineRenderer>" << std::endl;
}

void SingleLineRenderer::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetMaterial( AssetsManager::GetAsset<Material>(FileReader::ReadString(f)));
    SetOrigin(FileReader::ReadVec3(f));
    SetDestiny(FileReader::ReadVec3(f));
    SetLineWidth(FileReader::ReadFloat(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif

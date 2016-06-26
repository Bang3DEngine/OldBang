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

ICloneable *CircleRenderer::Clone() const
{
    return new CircleRenderer();
}

std::string CircleRenderer::GetName() const { return "CircleRenderer"; }

void CircleRenderer::GeneratePoints()
{
    points.clear();
    points.resize(segments + 1);

    float step = (2.0f * 3.141592f) / (segments);
    for(int i = 0;  i < segments + 1; ++i)
    {
        points[i] = Vector3(glm::cos(step*i), glm::sin(step*i), 0.0f) * radius;
    }
}

float CircleRenderer::GetDistanceInScreenSpace(const glm::vec2 &sOrigin,
                                              int pointIndex,
                                              const Matrix4 &modelViewProjMatrix) const
{
    Vector3 objP = points[pointIndex];
    glm::vec4 sP_4 = modelViewProjMatrix * glm::vec4(objP, 1.0f);
    glm::vec2 sP = sP_4.xy() / sP_4.w;
    sP = sP * 0.5f + 0.5f;

    return glm::distance(sOrigin, sP);
}

void CircleRenderer::GetTwoClosestPointsInScreenSpace(
        const glm::vec2 &sOrigin,
        const Matrix4 &modelViewProjMatrix,
        glm::vec2 *p0, int *i0,
        glm::vec2 *p1, int *i1) const
{
    /**
     * @brief We calculate this step in order to avoid sampling
     * points that are too close to each other, since this will
     * make the rotation axis losing precision in screen terms...
     */

    int step = points.size() / 8; // Sample 8 circle points evenly spaced
    step = glm::max(step, 1); // In case segments < 8

    float d0, d1; d0 = d1 = 99999.9f;
    for (int i = 0; i < points.size() - 1; i += step) // -1 because the last point is repeated
    {
        Vector3 objP = points[i];
        glm::vec4 sP_4 = modelViewProjMatrix * glm::vec4(objP, 1.0f);
        glm::vec2 sP = sP_4.xy() / sP_4.w;
        sP = sP * 0.5f + 0.5f;
        float d = glm::distance(sP, sOrigin);
        if(d < d0)
        {
            *p1 = *p0;
            *i1 = *i0;
            d1 = d0;

            *p0 = sP;
            *i0 = i;
            d0 = d;
        }
        else if(d < d1)
        {
            *p1 = sP;
            *i1 = i;
            d1 = d;
        }
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

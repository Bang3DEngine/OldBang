#include "Camera.h"
#include "Canvas.h"
#include "FileReader.h"

Camera::Camera() : orthoRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f)),
                   fovDegrees(60.0f),
                   zNear(0.1f),
                   zFar(100.0f),
                   aspectRatio(1.0f),
                   projMode(ProjectionMode::Perspective),
                   autoUpdateAspectRatio(true)
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo( "FOV", {fovDegrees} ),
        new InspectorVFloatSWInfo( "Z Near", {zNear} ),
        new InspectorVFloatSWInfo( "Z Far", {zFar} ),
        new InspectorVFloatSWInfo( "Aspect Ratio", {aspectRatio} ),
        new InspectorEnumSWInfo( "Projection Mode", {"Orthographic", "Perspective"} )
    });
    #endif
}

void Camera::GetViewMatrix(Matrix4 &view) const
{
    Transform *t = GetOwner()->GetComponent<Transform>();
    if(CAN_USE_COMPONENT(t))
    {
        t->GetModelMatrix(view);
        view = view.Inversed();
    }
    else
    {
        Logger_Verbose(GetOwner() << " has a Camera but does not have a transform. " <<
                       "View matrix will be the idgameObject matrix.");

        view = Matrix4(1.0f);
    }
}

void Camera::GetProjectionMatrix(Matrix4 &proj) const
{
    if(projMode == ProjectionMode::Perspective)
    {
        if(autoUpdateAspectRatio)
        {
            aspectRatio = Canvas::GetAspectRatio();
        }

        proj = Matrix4::Perspective(glm::radians(fovDegrees), aspectRatio, zNear, zFar);
    }
    else //Ortho
    {
        proj = Matrix4::Ortho(orthoRect.minx, orthoRect.maxx,
                              orthoRect.miny, orthoRect.maxy,
                              zNear, zFar);
    }
}

void Camera::SetOrthoRect(const Rect &rect)
{
    orthoRect = rect;
}







void Camera::SetFovDegrees(float fovDegrees)
{
    this->fovDegrees = fovDegrees;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

void Camera::SetZNear(float zNear)
{
    this->zNear = zNear;
}

void Camera::SetZFar(float zFar)
{
    this->zFar = zFar;
}

void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    this->projMode = projMode;
}

void Camera::SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio)
{
    this->autoUpdateAspectRatio = autoUpdateAspectRatio;
}






float Camera::GetFovDegrees() const
{
    return fovDegrees;
}

float Camera::GetAspectRatio() const
{
    return aspectRatio;
}

float Camera::GetZNear() const
{
    return zNear;
}

float Camera::GetZFar() const
{
    return zFar;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
    return projMode;
}

bool Camera::GetAutoUpdateAspectRatio() const
{
    return autoUpdateAspectRatio;
}

glm::vec2 Camera::WorldToScreenNDCPoint(const Vector3 &position)
{
    Matrix4 p, v;
    GetProjectionMatrix(p);
    GetViewMatrix(v);
    glm::vec4 v_4 = p * v * glm::vec4(position, 1);
    v_4 /= v_4.w;

    return v_4.xy();
}

const std::string Camera::ToString() const
{
    return "Camera";
}

ICloneable *Camera::Clone() const
{
    return new Camera();
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* Camera::GetComponentInfo()
{
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(0))->value = {fovDegrees};
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(1))->value = {zNear};
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(2))->value = {zFar};
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(3))->value = {aspectRatio};
    static_cast<InspectorEnumSWInfo*>(inspectorComponentInfo.GetSlotInfo(4))->selectedValueIndex = projMode;

    return &inspectorComponentInfo;
}

void Camera::OnSlotValueChanged(InspectorWidget *source)
{
    fovDegrees = source->GetSWVectorFloatValue("FOV")[0];
    zNear = source->GetSWVectorFloatValue("Z Near")[0];
    zFar = source->GetSWVectorFloatValue("Z Far")[0];
    aspectRatio = source->GetSWVectorFloatValue("Aspect Ratio")[0];
    projMode =  static_cast<Camera::ProjectionMode>(source->GetSWSelectedEnumIndex("Projection Mode"));
}

void Camera::Write(std::ostream &f) const
{
    f << "<Camera>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::Write(fovDegrees, f);
    FileWriter::Write(zNear, f);
    FileWriter::Write(zFar, f);
    FileWriter::Write(projMode == ProjectionMode::Perspective ?
                          "Perspective" : "Orthographic", f);
    FileWriter::Write(orthoRect, f);
    f << "</Camera>" << std::endl;
}

void Camera::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetFovDegrees( FileReader::ReadFloat(f) );
    SetZNear( FileReader::ReadFloat(f) );
    SetZFar( FileReader::ReadFloat(f) );
    SetProjectionMode( FileReader::ReadString(f) == "Perspective" ?
                                            Camera::ProjectionMode::Perspective :
                                            Camera::ProjectionMode::Orthographic);
    SetOrthoRect( FileReader::ReadRect(f) );
    FileReader::ReadNextLine(f); //Consume close tag
}
#endif

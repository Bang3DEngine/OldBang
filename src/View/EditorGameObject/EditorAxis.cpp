#include "EditorAxis.h"

#include "Canvas.h"

EditorAxis::EditorAxis(EditorAxis::EditorAxisDirection dir)
{
    AddComponent<Transform>();

    oAxisDirection = EditorAxis::GetVectorFromDir(dir);

    material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/lines.bmat");
    material = new Material(*material);

    lineColor = oAxisDirection.Abs();

    SetName(name);
}

EditorAxis::~EditorAxis()
{
    delete material;
}

std::string EditorAxis::GetStringFromDir(EditorAxis::EditorAxisDirection dir)
{
    if(dir == EditorAxisDirection::X) return "X";
    else if (dir == EditorAxisDirection::Y) return "Y";
    else return "Z";
}

Vector3 EditorAxis::GetVectorFromDir(EditorAxis::EditorAxisDirection dir)
{
    if(dir == EditorAxisDirection::X) return Vector3::right;
    else if (dir == EditorAxisDirection::Y) return Vector3::up;
    else return Vector3::forward;
}

void EditorAxis::GetMatrices(Matrix4 &pvm, Matrix4 &pv,
                             Matrix4 &p, Matrix4 &v, Matrix4 &m) const
{
    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    cam->GetProjectionMatrix(p);
    cam->GetViewMatrix(v);

    Transform *t = GetComponent<Transform>(); NONULL(t);
    t->GetModelMatrix(m);

    pv = p * v;
    pvm =  pv * m;
}

void EditorAxis::OnUpdate()
{
    // Process grabbing
    if(mouseIsOver && Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        // User has clicked on me!
        grabbed = true;
    }

    if(grabbed)
    {
        if(Input::GetMouseButtonUp(Input::MouseButton::MLeft))
        {
            grabbed = false;
            Input::SetMouseWrapping(false);
        }
        else
        {
            Input::SetMouseWrapping(true);
        }
    }

    // Process line color
    if (mouseIsOver)
    {
        GetAxisRenderer()->SetLineWidth(axisLineWidth * 1.5f);

        if (grabbed)
        {
            material->SetDiffuseColor(glm::vec4(lineColor, 1));
        }
        else
        {
            material->SetDiffuseColor(glm::vec4(lineColor * lineColorFadingNotGrabbed, 1));
        }
    }
    else
    {
        if (grabbed)
        {
            material->SetDiffuseColor(glm::vec4(lineColor, 1));
        }
        else
        {
            GetAxisRenderer()->SetLineWidth(axisLineWidth);
            material->SetDiffuseColor(glm::vec4(lineColor * lineColorFadingNotHover, 1));
        }
    }
}

void EditorAxis::OnMouseEnter()
{
    mouseIsOver = true;
}

void EditorAxis::OnMouseExit()
{
    mouseIsOver = false;
}

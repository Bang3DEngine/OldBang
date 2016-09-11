#include "DirectionalLight.h"

#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"


DirectionalLight::DirectionalLight() : Light()
{
    m_lightMaterialScreen = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_DirectionalLight_Screen.bmat");
    m_lightMaterialMesh   = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_DirectionalLight_Mesh.bmat");
}

const String DirectionalLight::ToString() const
{
    return "DirectionalLight";
}

String DirectionalLight::GetName() const
{
    return "DirectionalLight";
}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    DirectionalLight *dl = static_cast<DirectionalLight*>(clone);
}

ICloneable *DirectionalLight::Clone() const
{
    DirectionalLight *dl = new DirectionalLight();
    CloneInto(dl);
    return dl;
}

#ifdef BANG_EDITOR
void DirectionalLight::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void DirectionalLight::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void DirectionalLight::OnDrawGizmos()
{
    Light::OnDrawGizmos();

    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>("Assets/Engine/Textures/DirectionalLightIcon.btex2d");
    Gizmos::SetColor(Color(GetColor(), 0.5f));
    Gizmos::SetPosition(transform->GetPosition());
    Gizmos::SetScale(Vector3::one * 0.1f);
    Gizmos::RenderIcon(tex);

    if (gameObject->IsSelectedInHierarchy())
    {
        // Draw lines to show light's direction
        Camera *cam = Scene::GetCamera();
        float distScale = Vector3::Distance(cam->transform->GetPosition(),
                                            transform->GetPosition());

        const float radius = 0.03f * distScale;
        const float length = 0.2f * distScale;
        const Vector3 up = transform->GetUp() * radius;
        const Vector3 right = transform->GetRight() * radius;
        const Vector3 forward = transform->GetForward() * length;
        const Vector3 c = transform->GetPosition();

        Gizmos::SetColor(GetColor());
        Gizmos::SetReceivesLighting(false);
        for (float angle = 0.0f; angle <= 2 * glm::pi<float>(); angle += glm::pi<float>() / 4.0f)
        {
            Vector3 offx = right * glm::cos(angle);
            Vector3 offy = up * glm::sin(angle);
            Gizmos::SetColor(GetColor());
            Gizmos::RenderLine(c + offx + offy, c + offx + offy + forward);
        }
    }
}
#endif


void DirectionalLight::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Light::ReadXMLInfo(xmlInfo);
}

void DirectionalLight::FillXMLInfo(XMLNode *xmlInfo) const
{
    Light::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("DirectionalLight");
}

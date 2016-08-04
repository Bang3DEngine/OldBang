#include "DirectionalLight.h"

#include "Transform.h"
#include "GameObject.h"

DirectionalLight::DirectionalLight() : Light()
{
    m_lightMaterial = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_DirectionalLight.bmat");

    #ifdef BANG_EDITOR
    // m_inspectorComponentInfo.AddSlotInfos();
    #endif
}

const std::string DirectionalLight::ToString() const
{
    return "DirectionalLight";
}

std::string DirectionalLight::GetName() const
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
    Gizmos::DrawIcon(tex, gameObject->transform->GetPosition(), Vector3::one * 5.0f);

    if (gameObject->IsSelectedInHierarchy())
    {
        // Draw lines to show light's direction
        const float wide = 1.0f;
        const float length = 8.0f;
        const Vector3 up = transform->GetUp() * wide;
        const Vector3 right = transform->GetRight() * wide;
        const Vector3 forward = transform->GetForward() * length;
        const Vector3 c = transform->GetPosition();

        Gizmos::SetColor(GetColor());
        Gizmos::SetReceivesLighting(false);
        for (float angle = 0.0f; angle <= 2 * glm::pi<float>(); angle += glm::pi<float>() / 4.0f)
        {
            Vector3 offx = right * glm::cos(angle);
            Vector3 offy = up * glm::sin(angle);
            Gizmos::DrawLine(c + offx + offy, c + offx + offy + forward);
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

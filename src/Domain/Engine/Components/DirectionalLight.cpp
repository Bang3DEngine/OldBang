#include "DirectionalLight.h"

#include "Scene.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Material.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "InspectorWidget.h"
#endif

DirectionalLight::DirectionalLight()
{
    m_lightMaterialScreen = AssetsManager::Load<Material>(
                "Materials/SP_DirectionalLight_Screen.bmat", true);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    DirectionalLight *dl = Object::SCast<DirectionalLight>(clone);
}

void DirectionalLight::OnDrawGizmos(bool depthed, bool overlay)
{
    Light::OnDrawGizmos(depthed, overlay);

    if (!depthed && !overlay)
    {
        Texture2D *tex = AssetsManager::Load<Texture2D>(
                    "Textures/DirectionalLightIcon.btex2d", true);
        Gizmos::SetColor(Color(GetColor(), 1.0f));
        Gizmos::SetPosition(transform->GetPosition());
        Gizmos::SetScale(Vector3::One * 0.1f);
        Gizmos::RenderIcon(tex);

        if (gameObject->IsSelected())
        {
            // Draw lines to show light's direction
            Camera *cam = SceneManager::GetActiveScene()->GetCamera();
            float distScale = Vector3::Distance(cam->transform->GetPosition(),
                                                transform->GetPosition());

            const float radius = 0.03f * distScale;
            const float length = 0.2f * distScale;
            const Vector3 up = transform->GetUp() * radius;
            const Vector3 right = transform->GetRight() * radius;
            const Vector3 forward = transform->GetForward() * length;
            const Vector3 c = transform->GetPosition();

            Gizmos::SetReceivesLighting(false);
            for (float ang = 0.0f; ang <= 2 * Math::PI; ang += Math::PI / 4.0f)
            {
                Vector3 offx = right * Math::Cos(ang);
                Vector3 offy = up * Math::Sin(ang);
                Gizmos::SetColor(Color(GetColor(), 1.0f));
                Gizmos::RenderLine(c + offx + offy, c + offx + offy + forward);
            }
        }
    }
}

void DirectionalLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
}

void DirectionalLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
    xmlInfo->SetTagName("DirectionalLight");
}

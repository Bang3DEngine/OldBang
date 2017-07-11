#include "Bang/DirectionalLight.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"

DirectionalLight::DirectionalLight()
{
    m_lightMaterialScreen = AssetsManager::Load<Material>(
                EPATH("Materials/SP_DirectionalLight_Screen.bmat") );
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
                    EPATH("Textures/DirectionalLightIcon.btex2d") );
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
}

#include "Bang/Settings.h"

#include "Bang/Application.h"

USING_NAMESPACE_BANG

void Settings::SetAmbientLight(float ambientLight)
{
    Settings::GetInstance()->m_ambientLight = ambientLight;
}

float Settings::GetAmbientLight()
{
    return Settings::GetInstance()->m_ambientLight;
}

Settings *Settings::GetInstance()
{
    return Application::GetInstance()->GetSettings();
}

Settings::Settings()
{
}

Settings::~Settings()
{
}


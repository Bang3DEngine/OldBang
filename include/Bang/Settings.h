#ifndef SETTINGS_H
#define SETTINGS_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Settings
{
public:

    static void SetAmbientLight(float ambientLight);

    static float GetAmbientLight();

    static Settings *GetInstance();

private:
    float m_ambientLight = 0.1f;

	Settings();
	virtual ~Settings();

    friend class Application;
};

NAMESPACE_BANG_END

#endif // SETTINGS_H


#ifndef LOOKATLUIGI_H
#define LOOKATLUIGI_H

#include "Behaviour.h"

class LookAtLuigi : public Behaviour
{
private:
    float angle = 0.0f;

public:
    LookAtLuigi();
    virtual ~LookAtLuigi();

    void OnStart() override;
    void OnUpdate() override;
};

BANG_BEHAVIOUR_CLASS(LookAtLuigi)

#endif // LOOKATLUIGI_H

#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include "Behaviour.h"

class TestBehaviour : public Behaviour
{
private:
    float time;
    float originalScale;
    glm::vec3 randomAxis;

public:
    TestBehaviour();

    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;
};

#endif // TESTBEHAVIOUR_H

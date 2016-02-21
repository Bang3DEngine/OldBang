#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include "Behaviour.h"

class TestBehaviour : public Behaviour
{
private:
    float time;

public:
    TestBehaviour();

    void OnUpdate() override;
};

#endif // TESTBEHAVIOUR_H

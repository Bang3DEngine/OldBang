#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include "Logger.h"
#include "Transform.h"
#include "Behaviour.h"
#include "Canvas.h"

class TestBehaviour : public Behaviour
{
private:
    float time;

public:
    TestBehaviour();

    void OnUpdate() override;
};

#endif // TESTBEHAVIOUR_H

#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include "Logger.h"
#include "Transform.h"
#include "Behaviour.h"

class TestBehaviour : public Behaviour
{
public:
    TestBehaviour();

    void OnUpdate() override;
};

#endif // TESTBEHAVIOUR_H

#ifndef TESTCAMERABEHAVIOUR_H
#define TESTCAMERABEHAVIOUR_H

#include "Behaviour.h"

class TestCameraBehaviour : public Behaviour
{
private:
    float time;

public:
    TestCameraBehaviour();

    void OnUpdate() override;
};

#endif // TESTCAMERABEHAVIOUR_H

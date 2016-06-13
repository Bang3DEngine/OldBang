#ifndef TESTCAMERABEHAVIOUR_H
#define TESTCAMERABEHAVIOUR_H

#include "Behaviour.h"

class TestCameraBehaviour : public Behaviour
{
private:
    float time;

public:
    TestCameraBehaviour();
    virtual ~TestCameraBehaviour();

    void OnStart() override;
    void OnUpdate() override;
};

BANG_BEHAVIOUR_CLASS(TestCameraBehaviour)

#endif // TESTCAMERABEHAVIOUR_H

#include "TestCameraBehaviour.h"

#include "WindowMain.h"

#include "GameObject.h"
#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Scene.h"
#include "Box.h"
#include "Time.h"

TestCameraBehaviour::TestCameraBehaviour()
{
    //this->SetFilepath(__FILE__);
    //time = 0.0f;
}


void TestCameraBehaviour::OnUpdate()
{
    GetOwner()->GetComponent<Transform>()->Translate(
                GetOwner()->GetComponent<Transform>()->GetForward()
                );
}

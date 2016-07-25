#include "LookAtLuigi.h"

#include "ShaderContract.h"
#include "Transform.h"
#include "Material.h"
#include "Logger.h"
#include "Canvas.h"
#include "Scene.h"
#include "Time.h"

#ifdef BANG_EDITOR
#include "WindowMain.h"
#else
#include "GameWindow.h"
#endif

LookAtLuigi::LookAtLuigi()
{
}

LookAtLuigi::~LookAtLuigi()
{
}

void LookAtLuigi::OnStart()
{
    Behaviour::OnStart();
}


void LookAtLuigi::OnUpdate()
{
    Behaviour::OnUpdate();

    //GameObject *luigi = GameObject::Find("LuigiHolder");
    GameObject *luigi = GameObject::Find("Luigi");
    if (luigi)
    {
        //Vector3 myPos = gameObject->GetTransform()->GetPosition() * Vector3(1,0,1);
        //Vector3 luigiPos = luigi->GetTransform()->GetPosition() * Vector3(1,0,1);
        //gameObject->GetTransform()->LookInDirection(luigiPos - myPos);
    }

    if(Input::GetKeyDown(Input::Key::X))
    {
        // Logger_Log("MY PARENT: " << gameObject->GetParent());
        // Logger_Log("LUIGIS xPARENT: " << luigi->GetParent());
    }
}



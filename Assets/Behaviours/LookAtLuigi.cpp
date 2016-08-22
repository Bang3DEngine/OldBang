#include "LookAtLuigi.h"

#include "ShaderContract.h"
#include "Transform.h"
#include "Material.h"
#include "Logger.h"
#include "Screen.h"
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

    GameObject *luigi = GameObject::Find("Character");
    if (luigi)
    {
        Vector3 myPos = gameObject->transform->GetPosition();
        Vector3 luigiPos = luigi->transform->GetPosition();
        gameObject->transform->LookInDirection(luigiPos - myPos);
    }

    if(Input::GetKeyDown(Input::Key::X))
    {
    }
}



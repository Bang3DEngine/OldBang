#include "Bang/UITree.h"

#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UITree::UITree()
{
}

UITree::~UITree()
{
}

void UITree::OnUpdate()
{
    Component::OnUpdate();
    if (Input::GetKeyDown(Key::A))
    {
        GetList()->AddElement( GameObjectFactory::CreateUIImage(
                                 Color::Blue, Vector2i(50))->GetGameObject() );
    }
}

UIList *UITree::GetList() const { return p_list; }

UITree *UITree::CreateInto(GameObject *go)
{
    UIList *list = GameObjectFactory::CreateUIListInto(go);

    UITree *tree = go->AddComponent<UITree>();
    tree->p_list = list;
}

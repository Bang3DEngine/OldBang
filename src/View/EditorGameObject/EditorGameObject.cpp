#include "EditorGameObject.h"

EditorGameObject::EditorGameObject() : GameObject()
{

}

EditorGameObject::~EditorGameObject()
{

}

#ifdef BANG_EDITOR
void EditorGameObject::Write(std::ostream &f) const
{

}

void EditorGameObject::Read(std::istream &f)
{

}
#endif

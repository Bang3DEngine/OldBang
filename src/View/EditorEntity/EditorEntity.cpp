#include "EditorEntity.h"

EditorEntity::EditorEntity() : Entity()
{

}

EditorEntity::~EditorEntity()
{

}

#ifdef BANG_EDITOR
void EditorEntity::Write(std::ostream &f) const
{

}

void EditorEntity::Read(std::istream &f)
{

}
#endif

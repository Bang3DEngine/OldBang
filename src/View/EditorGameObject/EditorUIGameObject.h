#ifndef EDITORUIGAMEOBJECT_H
#define EDITORUIGAMEOBJECT_H


#include "UIGameObject.h"
#include "EditorGameObject.h"

class EditorUIGameObject : public EditorGameObject,
                           public UIGameObject
{
public:
    EditorUIGameObject();
    virtual ~EditorUIGameObject();

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable* Clone() const override;

    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

};

#endif // EDITORUIGAMEOBJECT_H

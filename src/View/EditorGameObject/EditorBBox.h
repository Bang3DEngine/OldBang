#ifndef EDITORBBOX_H
#define EDITORBBOX_H

#include "Array.h"
#include "Vector3.h"

#include "GameObject.h"

class Material;
class GameObject;
class EditorBBox : public GameObject
{
private:
    static Array<Vector3> s_boxVertices;
    static Material *s_boxMaterial;

    GameObject *m_attachedGameObject = nullptr;

public:
    EditorBBox(GameObject *attachedGameObject);

    void OnUpdate() override;
};

#endif // EDITORBBOX_H

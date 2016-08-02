#ifndef EDITORBBOX_H
#define EDITORBBOX_H

#include "Bang.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"
#include "EditorGameObject.h"

class EditorBBox : public EditorGameObject
{
private:
    static std::vector<Vector3> s_boxVertices;
    static Material *s_boxMaterial;

    GameObject *m_attachedGameObject = nullptr;

public:
    EditorBBox(GameObject *attachedGameObject);

    void OnUpdate() override;
};

#endif // EDITORBBOX_H

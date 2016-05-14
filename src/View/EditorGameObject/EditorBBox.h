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
    static std::vector<glm::vec3> boxVertices;

public:
    EditorBBox();

    void OnUpdate() override;
};

#endif // EDITORBBOX_H

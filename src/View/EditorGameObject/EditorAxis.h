#ifndef EDITORAXIS_H
#define EDITORAXIS_H

#include "Transform.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "EditorGameObject.h"

class EditorAxis : public EditorGameObject
{
private:
    Material *matx = nullptr;
    Material *maty = nullptr;
    Material *matz = nullptr;

    Transform *transform = nullptr;
    LineRenderer *xAxisLine = nullptr;
    LineRenderer *yAxisLine = nullptr;
    LineRenderer *zAxisLine = nullptr;

public:
    EditorAxis();
    virtual ~EditorAxis();
};

#endif // EDITORAXIS_H

#ifndef FILEREADER_H
#define FILEREADER_H

#include "Bang.h"

FORWARD   class Scene;

class FileReader
{
public:
    static int GetModelNumTriangles(const Path& filepath);
    static bool ReadModel(const Path& filepath,
                          Array<Vector3> *vertexPos,
                          Array<Vector3> *vertexNormals,
                          Array<Vector2> *vertexUvs);
private:
    FileReader() {}
};

#endif // FILEREADER_H

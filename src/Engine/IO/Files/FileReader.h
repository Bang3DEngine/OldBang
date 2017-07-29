#ifndef FILEREADER_H
#define FILEREADER_H

#include "Bang.h"

FORWARD   class Path;
FORWARD_T class Array;
FORWARD   class Scene;
FORWARD   class GameObject;
FORWARD_T class Vector2G;
FORWARD_T class Vector3G;

class FileReader
{
public:
    static int GetModelNumTriangles(const Path& filepath);
    static bool ReadModel(const Path& filepath,
                          Array< Vector3G<float> > *vertexPos,
                          Array< Vector3G<float> > *vertexNormals,
                          Array< Vector2G<float> > *vertexUvs);
private:
    FileReader() {}
};

#endif // FILEREADER_H

#ifndef FILEREADER_H
#define FILEREADER_H

#include "Bang/Array.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/Rect.h"

class Scene;
class GameObject;
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

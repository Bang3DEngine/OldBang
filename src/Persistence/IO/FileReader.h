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
#include "Bang/Rect.h"
#include "Bang/Debug.h"

class Scene;
class GameObject;
class FileReader
{
public:
    static int GetMeshNumTriangles(const String& filepath);
    static bool ReadMesh(const String& filepath,
                        Array<Vector3> *vertexPos,
                        Array<Vector3> *vertexNormals,
                        Array<Vector2> *vertexUvs);
private:
    FileReader() {}
};

#endif // FILEREADER_H

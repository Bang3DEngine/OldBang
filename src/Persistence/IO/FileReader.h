#ifndef FILEREADER_H
#define FILEREADER_H

#include "Array.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "Map.h"
#include "Rect.h"
#include "Debug.h"

class Scene;
class GameObject;
class FileReader
{
public:
    static unsigned char* ReadImage(const String& filepath,
                                    int *width, int *height);

    static void GetOBJFormat(const String& filepath, bool *hasUvs,
                             bool *hasNormals, bool *isTriangles);
    static int GetOBJNumFaces(const String& filepath);
    static bool ReadOBJ(const String& filepath,
                        Array<Vector3> *vertexPos,
                        Array<Vector3> *vertexNormals,
                        Array<Vector2> *vertexUvs,
                        bool *isTriangles);
private:
    FileReader() {}
};

#endif // FILEREADER_H

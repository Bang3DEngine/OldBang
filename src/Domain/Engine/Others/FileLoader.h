#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "Logger.h"
#include "stb_image.h"
#include "glm/glm.hpp"

class FileLoader
{

private:

    static void GetOBJFormat(const std::string& filepath, bool *hasUvs, bool *hasNormals, bool *isTriangles);

public:

    static unsigned char* LoadImage(const std::string& filepath, int *components, int *width, int *height);

    static bool LoadOBJ(const std::string& filepath,
                        std::vector<glm::vec3> *vertexPos,
                        std::vector<glm::vec3> *vertexNormals,
                        std::vector<glm::vec2> *vertexUvs,
                        bool *isTriangles);
};

#endif // FILELOADER_H

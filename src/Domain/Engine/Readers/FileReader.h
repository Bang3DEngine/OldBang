#ifndef FILEREADER_H
#define FILEREADER_H

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

class Stage;
class FileReader
{
private:
    static void GetOBJFormat(const std::string& filepath, bool *hasUvs, bool *hasNormals, bool *isTriangles);

public:

    static unsigned char* ReadImage(const std::string& filepath, int *components, int *width, int *height);

    static bool ReadOBJ(const std::string& filepath,
                        std::vector<glm::vec3> *vertexPos,
                        std::vector<glm::vec3> *vertexNormals,
                        std::vector<glm::vec2> *vertexUvs,
                        bool *isTriangles);


};

#endif // FILEREADER_H

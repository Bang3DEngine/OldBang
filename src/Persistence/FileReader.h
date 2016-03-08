#ifndef FILEREADER_H
#define FILEREADER_H

#include "Bang.h"

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "stb_image.h"
#include "glm/glm.hpp"

#include "Rect.h"
#include "Logger.h"

class Stage;
class FileReader
{
private:
    FileReader() {}

    static void GetOBJFormat(const std::string& filepath, bool *hasUvs, bool *hasNormals, bool *isTriangles);

    static void TrimStringLeft(std::string *str);

public:

    static unsigned char* ReadImage(const std::string& filepath, int *components, int *width, int *height);

    static bool ReadOBJ(const std::string& filepath,
                        std::vector<glm::vec3> *vertexPos,
                        std::vector<glm::vec3> *vertexNormals,
                        std::vector<glm::vec2> *vertexUvs,
                        bool *isTriangles);

    //READ PRIMITIVE VALUES
    static std::string ReadNextLine(std::istream &f);

    static float ReadFloat(std::istream &f);
    static glm::vec2 ReadVec2(std::istream &f);
    static glm::vec3 ReadVec3(std::istream &f);
    static glm::quat ReadQuat(std::istream &f);
    static Rect ReadRect(std::istream &f);
    static std::string ReadString(std::istream &f);


};

#endif // FILEREADER_H

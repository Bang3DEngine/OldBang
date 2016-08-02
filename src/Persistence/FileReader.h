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

class Scene;
class GameObject;
class FileReader
{
private:
    FileReader() {}

    static const std::string NoRegisterId;
    static void *lastIstreamDir;

public:

    //IMAGE STUFF
    static void GetImageFormat(const std::string& filepath, int *width,
                               int *height, int *numComponents);
    static unsigned char* ReadImage(const std::string& filepath,
                                    int *width, int *height);
    //


    //3D STUFF
    static void GetOBJFormat(const std::string& filepath, bool *hasUvs,
                             bool *hasNormals, bool *isTriangles);

    static int GetOBJNumFaces(const std::string& filepath);

    static bool ReadOBJ(const std::string& filepath,
                        std::vector<Vector3> *vertexPos,
                        std::vector<Vector3> *vertexNormals,
                        std::vector<glm::vec2> *vertexUvs,
                        bool *isTriangles);
    //


    //SCENE AND GAMEOBJECTS RELATED STUFF

    //They all start from the first line inside the tag
    // (you must consume the initial tag),
    // and they consume the closing tag.

    //Scene must be a created object before being passed to these functions (new Scene)
    static void ReadScene(const std::string &filepath, Scene* scene);
    //

    //READ PRIMITIVE VALUES
    static std::string PeekNextLine(std::istream &f);
    static std::string ReadNextLine(std::istream &f);
    static bool ReadNextLine(std::istream &f, std::string *line);

    static bool ReadBool(std::istream &f);
    static int ReadInt(std::istream &f);
    static float ReadFloat(std::istream &f);
    static glm::vec2 ReadVec2(std::istream &f);
    static Vector3 ReadVec3(std::istream &f);
    static glm::vec4 ReadVec4(std::istream &f);
    static Quaternion ReadQuat(std::istream &f);
    static Rect ReadRect(std::istream &f);
    static std::string ReadString(std::istream &f);
};

#endif // FILEREADER_H

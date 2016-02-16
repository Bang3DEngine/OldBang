#ifndef STAGELOADER_H
#define STAGELOADER_H

#include <istream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <stack>
#include <map>

#include "glm/glm.hpp"

#include "MeshPyramid.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Logger.h"
#include "Stage.h"
#include "Mesh.h"

class StageLoader
{
private:

    //For every id(first line after a tag in the file)
    // it saves a pointer to the corresponding Part, Entity, etc.
    static std::map<std::string, void*> idToPointers;

    StageLoader() {}

    static void TrimStringLeft(std::string *str);
    static std::string GetLine(std::ifstream &f);


    //READ/REGISTER POINTERS
    template <class T>
    static T* ReadNextPointer(std::ifstream &f)
    {
        std::string id = ReadString(f);
        if(idToPointers.find(id) == idToPointers.end()) return nullptr;
        return (T*)(idToPointers[id]);
    }
    static void RegisterNextPointer(std::ifstream &f, void *pointer);


    //READ VALUES
    static float ReadFloat(std::ifstream &f);
    static glm::vec2 ReadVec2(std::ifstream &f);
    static glm::vec3 ReadVec3(std::ifstream &f);
    static glm::quat ReadQuat(std::ifstream &f);
    static std::string ReadString(std::ifstream &f);

    //They all start from the first line inside the tag
    // (you must consume the initial tag),
    // and they consume the closing tag.

    //PARTS
    static void ReadParts(std::ifstream &f, Entity **e);
    static Transform* ReadTransform(std::ifstream &f);
    static MeshPyramid* ReadMeshPyramid(std::ifstream &f);
    static MeshRenderer* ReadMeshRenderer(std::ifstream &f);

    //ASSETS
    static void ReadAssets(std::ifstream &f);
    static Material* ReadMaterial(std::ifstream &f);

    //ENTITIES
    static void ReadChildren(std::ifstream &f, Entity **e);
    static Entity* ReadEntity(std::ifstream &f);
    // ////////////////////

public:
    static void LoadStage(const std::string &filepath, Stage** stage);
    static void SaveStage(const std::string &filepath, const Stage *stage);
};

#endif // STAGELOADER_H

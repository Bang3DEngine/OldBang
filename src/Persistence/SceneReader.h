#ifndef SCENEREADER_H
#define SCENEREADER_H

#include "Bang.h"

#include <istream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <stack>
#include <map>

#include "ReaderWithPointerIds.h"
#include "glm/glm.hpp"
#include "Rect.h"

#include "BP_SceneReader_cpp_UserBehaviours_elseifs.bp"

class Scene;
class GameObject;
class SceneReader : public ReaderWithPointerIds
{
private:

    SceneReader() {}

public:

    //They all start from the first line inside the tag
    // (you must consume the initial tag),
    // and they consume the closing tag.

    //COMPONENTS
    static void ReadComponents(std::istream &f, GameObject *e);

    //ENTITIES
    static void ReadChildren(std::istream &f, GameObject *e);
    // ////////////////////

public:
    static void ReadScene(const std::string &filepath, Scene* scene);
    static void SaveScene(const std::string &filepath, const Scene *scene);

};

#endif // SCENEREADER_H

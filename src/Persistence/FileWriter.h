#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>

#include <string>

#include "Vector3.h"
#include "Rect.h"
#include "Persistence.h"

class Scene;
class Asset;
class FileWriter
{
private:
    FileWriter();

public:
    static void WriteScene(std::string filename, Scene *scene);
    static void WriteAsset(std::string filename, Asset *a);

    static void WriteBool(bool v, std::ostream &f);
    static void Write(int v, std::ostream &f);
    static void Write(float v, std::ostream &f);
    static void Write(void *v, std::ostream &f);
    static void Write(const glm::vec2 &v, std::ostream &f);
    static void Write(const Vector3 &v, std::ostream &f);
    static void Write(const glm::vec4 &v, std::ostream &f);
    static void Write(const Quaternion &q, std::ostream &f);
    static void Write(const Rect &r, std::ostream &f);
    static void Write(const std::string &str, std::ostream &f);
    static void WriteFilepath(const std::string &path, std::ostream &f);
};

#endif // FILEWRITER_H

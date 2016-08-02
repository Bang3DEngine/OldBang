#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>

#include <string>

#include "Vector3.h"
#include "Rect.h"
#include "Persistence.h"
#include "XMLParser.h"
#include "XMLNode.h"

class Scene;
class Asset;
class FileWriter
{
private:
    FileWriter();

public:
    static void WriteScene(const std::string &filepath, Scene *scene);
    static void WriteAsset(const std::string &filepath, Asset *a);
    static void WriteToFile(const std::string &filepath, const std::string &content);

    static void WriteXMLNode(XMLNode *xmlInfo, std::ostream &f);
    static void WriteBool(bool v, std::ostream &f);
    static void WriteInt(int v, std::ostream &f);
    static void WriteFloat(float v, std::ostream &f);
    static void WritePointer(void *v, std::ostream &f);
    static void WriteVector2(const glm::vec2 &v, std::ostream &f);
    static void WriteVector3(const Vector3 &v, std::ostream &f);
    static void WriteVector4(const Vector4 &v, std::ostream &f);
    static void WriteQuaternion(const Quaternion &q, std::ostream &f);
    static void WriteRect(const Rect &r, std::ostream &f);
    static void WriteString(const std::string &str, std::ostream &f);
    static void WriteFilepath(const std::string &path, std::ostream &f);
};

#endif // FILEWRITER_H

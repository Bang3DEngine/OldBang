#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>

#include <string>

#include "Rect.h"

class Stage;
class FileWriter
{
private:
    FileWriter();

public:
    static void SaveStage(std::string filename, Stage *stage);

    static void Write(float v, std::ostream &f);
    static void Write(const glm::vec2 &v, std::ostream &f);
    static void Write(const glm::vec3 &v, std::ostream &f);
    static void Write(const glm::quat &q, std::ostream &f);
    static void Write(const Rect &r, std::ostream &f);
    static void Write(const std::string &str, std::ostream &f);
};

#endif // FILEWRITER_H

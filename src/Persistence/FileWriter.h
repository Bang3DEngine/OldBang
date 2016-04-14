#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>

#include <string>


#include "Stage.h"

class FileWriter
{
private:
    FileWriter();

public:
    static void SaveStage(std::string filename, Stage *stage);
};

#endif // FILEWRITER_H

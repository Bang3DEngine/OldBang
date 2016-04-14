#include "FileWriter.h"

FileWriter::FileWriter()
{
}

void FileWriter::SaveStage(std::string filename, Stage *stage)
{
    std::ofstream ofs;
    ofs.open(filename);
    if(!ofs.is_open())
    {
        Logger_Error("There was an error when opening file '" << filename << "'");
        return;
    }

    stage->Write(ofs);

    ofs.close();
}

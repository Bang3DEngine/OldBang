#include "FileWriter.h"
#include "Stage.h"

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

void FileWriter::Write(float v, std::ostream &f)
{
    f << v << std::endl;
}

void FileWriter::Write(const glm::vec2 &v, std::ostream &f)
{
    f << v.x << " " << v.y << std::endl;
}

void FileWriter::Write(const glm::vec3 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << std::endl;
}

void FileWriter::Write(const glm::quat &q, std::ostream &f)
{
    f << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
}

void FileWriter::Write(const Rect &r, std::ostream &f)
{
    f << r.left << " " << r.right << " " << r.bottom << " " << r.top << std::endl;
}

void FileWriter::Write(const std::string &str, std::ostream &f)
{
    if(str != "")
    {
        f << str << std::endl;
    }
    else
    {
        f << "-" << std::endl;
    }
}

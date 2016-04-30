#include "FileWriter.h"
#include "Stage.h"
#include "Asset.h"

FileWriter::FileWriter()
{
}

void FileWriter::WriteStage(std::string filename, Stage *stage)
{
    std::ofstream ofs;
    filename = Persistence::AppendExtension(filename,
                                            Stage::GetFileExtension());
    ofs.open(filename);
    if(!ofs.is_open())
    {
        Logger_Error("There was an error when saving file '" << filename << "'");
        ofs.close();
        return;
    }
    stage->Write(ofs);
    ofs.close();
}

void FileWriter::WriteAsset(std::string filename, Asset *a)
{
    std::ofstream ofs;
    filename = Persistence::AppendExtension(filename,
                                            a->GetFileExtension());
    Logger_Log(a->GetFileExtensionStatic());
    ofs.open(filename);
    if(!ofs.is_open())
    {
        Logger_Error("There was an error when saving file '" << filename << "'");
        ofs.close();
        return;
    }
    a->Write(ofs);
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

void FileWriter::Write(const glm::vec4 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
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

void FileWriter::WriteFilepath(const std::string &path, std::ostream &f)
{
    Write(Persistence::ProjectRootAbsoluteToRelative(path), f);
}

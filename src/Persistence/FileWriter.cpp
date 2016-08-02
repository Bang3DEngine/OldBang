#include "FileWriter.h"
#include "Scene.h"
#include "Asset.h"

FileWriter::FileWriter()
{
}

void FileWriter::WriteScene(const std::string &filepath, Scene *scene)
{
    std::ofstream ofs;
    std::string fpath = Persistence::AppendExtension(filepath, Scene::GetFileExtension());
    ofs.open(fpath);
    if (ofs.is_open())
    {
        XMLNode *xmlInfo = new XMLNode();
        scene->FillXMLInfo(xmlInfo);
        ofs << xmlInfo->ToString();
    }
    else
    {
        Logger_Error("There was an error when saving file '" << fpath << "'");
    }
    ofs.close();
}

void FileWriter::WriteAsset(const std::string &filepath, Asset *a)
{
    std::ofstream ofs;
    std::string fpath = Persistence::AppendExtension(filepath, a->GetFileExtension());
    ofs.open(fpath);
    if (ofs.is_open())
    {
        XMLNode *xmlInfo = new XMLNode();
        a->FillXMLInfo(xmlInfo);
        ofs << xmlInfo->ToString();
    }
    else
    {
        Logger_Error("There was an error when saving file '" << filepath << "'");
    }
    ofs.close();
}

void FileWriter::WriteToFile(const std::string &filepath, const std::string &content)
{
    std::ofstream ofs;
    ofs.open(filepath);
    if (ofs.is_open())
    {
        ofs << content;
    }
    else
    {
        Logger_Error("There was an error when saving file '" << filepath << "'");
    }
    ofs.close();
}

void FileWriter::WriteXMLNode(XMLNode *xmlInfo, std::ostream &f)
{
    NONULL(xmlInfo);
    f << xmlInfo->ToString();
}

void FileWriter::WriteBool(bool v, std::ostream &f)
{
    std::string s = v ? "true" : "false";
    f << s << std::endl;
}

void FileWriter::WriteInt(int v, std::ostream &f)
{
    f << v << std::endl;
}

void FileWriter::WriteFloat(float v, std::ostream &f)
{
    f << v << std::endl;
}

void FileWriter::WritePointer(void *v, std::ostream &f)
{
    f << v << std::endl;
}

void FileWriter::WriteVector2(const Vector2 &v, std::ostream &f)
{
    f << v.x << " " << v.y << std::endl;
}

void FileWriter::WriteVector3(const Vector3 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << std::endl;
}

void FileWriter::WriteVector4(const Vector4 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
}


void FileWriter::WriteQuaternion(const Quaternion &q, std::ostream &f)
{
    f << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
}

void FileWriter::WriteRect(const Rect &r, std::ostream &f)
{
    f << r.m_minx << " " << r.m_maxx << " " << r.m_miny << " " << r.m_maxy << std::endl;
}

void FileWriter::WriteString(const std::string &str, std::ostream &f)
{
    if (str != "")
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
    FileWriter::WriteString(Persistence::ProjectRootAbsoluteToRelative(path), f);
}

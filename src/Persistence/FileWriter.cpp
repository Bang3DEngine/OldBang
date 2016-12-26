#include "FileWriter.h"

#include "Rect.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "Scene.h"
#include "Asset.h"
#include "Debug.h"
#include "Persistence.h"

FileWriter::FileWriter()
{
}

void FileWriter::WriteScene(const String &filepath, Scene *scene)
{
    String fpath = Persistence::AppendExtension(filepath, Scene::GetFileExtension());
    FileWriter::WriteToFile(fpath, scene->GetXMLInfoString());
}

void FileWriter::WriteAsset(const String &filepath, Asset *a)
{
    String fpath = Persistence::AppendExtension(filepath, a->GetFileExtension());
    FileWriter::WriteToFile(fpath, a->GetXMLInfoString());
}

bool FileWriter::WriteToFile(const String &filepath, const IFileable *fileable)
{
    return FileWriter::WriteToFile(filepath, fileable->GetXMLInfoString());
}

bool FileWriter::WriteToFile(const String &filepath, const String &content)
{
    bool ok = true;
    std::ofstream ofs;
    ofs.open(filepath);
    if (ofs.is_open())
    {
        ofs << content;
    }
    else
    {
        Debug_Error("There was an error when saving file '" << filepath << "'");
        ok = false;
    }
    ofs.close();
    return ok;
}

void FileWriter::WriteXMLNode(XMLNode *xmlInfo, std::ostream &f)
{
    NONULL(xmlInfo);
    f << xmlInfo->ToString();
}

void FileWriter::WriteBool(bool v, std::ostream &f)
{
    String s = v ? "true" : "false";
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

void FileWriter::WriteString(const String &str, std::ostream &f)
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

void FileWriter::WriteFilepath(const String &path, std::ostream &f)
{
    FileWriter::WriteString(Persistence::ToRelative(path), f);
}

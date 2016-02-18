#include "StageReader.h"

#include "Mesh.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Logger.h"
#include "Stage.h"

std::map<std::string, void*> StageReader::idToPointers;

void StageReader::TrimStringLeft(std::string *str)
{
    unsigned int i = 0;
    for(; i < str->length(); ++i)
    {
        if(str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(i, str->length() - i);
}

std::string StageReader::GetLine(std::ifstream &f)
{
    std::string line;
    do
    {
        std::getline(f, line);
        TrimStringLeft(&line);
    }
    while( line.empty() || line.at(0) == '#'); //Skip all empty/comment lines

    return line;
}

float StageReader::ReadFloat(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float v; iss >> v; return v;
}

glm::vec2 StageReader::ReadVec2(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y;
    iss >> x >> y;
    return glm::vec2(x, y);
}

glm::vec3 StageReader::ReadVec3(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y,z;
    iss >> x >> y >> z;
    return glm::vec3(x, y, z);
}

glm::quat StageReader::ReadQuat(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return glm::quat(x, y, z, w);
}

Rect StageReader::ReadRect(std::ifstream &f)
{
    glm::quat q = ReadQuat(f);
    return Rect(q.x, q.y, q.z, q.w);
}

std::string StageReader::ReadString(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    std::string str;
    iss >> str;
    return str;
}

void StageReader::RegisterNextPointer(std::ifstream &f, void *pointer)
{
    idToPointers[ ReadString(f) ] = pointer;
}

void StageReader::ReadParts(std::ifstream &f, Entity *e)
{
    std::string line;
    while( (line = GetLine(f)) != "</parts>" )
    {
        Part *p = nullptr;
        if(line == "<Transform>")
        {
            Transform *t = new Transform();
            t->Read(f);
            p = t;
        }
        else if(line == "<Behaviour>")
        {
            //p = ReadBehaviour(f);
        }
        else if(line == "<MeshRenderer>")
        {
            MeshRenderer *mr = new MeshRenderer();
            mr->Read(f);
            p = mr;
        }
        else if(line == "<Camera>")
        {
            Camera *cam = new Camera();
            cam->Read(f);
            p = cam;
        }


        if(p != nullptr)
        {
            e->AddPart(p);
        }
    }
}

void StageReader::ReadChildren(std::ifstream &f, Entity *e)
{
    std::string line;
    while( (line = GetLine(f)) != "</children>" )
    {
        Entity *child = new Entity();
        child->Read(f);
        e->AddChild(child);
    }
}

void StageReader::ReadAssets(std::ifstream &f)
{
    std::string line;
    while( (line = GetLine(f)) != "</assets>" )
    {
        if(line == "<Material>")
        {
            Material *m = new Material();
            m->Read(f);
        }
        else if(line == "<Mesh>")
        {
            Mesh *m = new Mesh();
            m->Read(f);
        }
    }
}

void StageReader::ReadStage(const std::string &filepath, Stage* stage)
{
    idToPointers.clear();

    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the stage.");
    }
    else
    {
        std::string line = GetLine(f); // Skip <Stage> line
        RegisterNextPointer(f, stage); // Read Stage id
        stage->SetName( ReadString(f) ); //Read Stage name

        while( (line = GetLine(f)) != "</Stage>")
        {
            if(line == "") continue; //Skip blank lines

            if(line == "<assets>")
            {
                ReadAssets(f);
            }
            else if(line == "<children>")
            {
                ReadChildren(f, (Entity*)stage);
            }
            else
            {

            }
        }
    }
}

void StageReader::SaveStage(const std::string &filepath, const Stage *stage)
{

}

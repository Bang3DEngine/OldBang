#include "StageLoader.h"

void StageLoader::TrimStringLeft(std::string *str)
{
    unsigned int i = 0;
    for(; i < str->length(); ++i)
    {
        if(str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(i, str->length() - i);
}

std::string StageLoader::GetLine(std::ifstream &f)
{
    std::string line;
    std::getline(f, line);
    TrimStringLeft(&line);
    return line;
}

float StageLoader::ReadFloat(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float v; iss >> v; return v;
}

glm::vec2 StageLoader::ReadVec2(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y;
    iss >> x >> y;
    return glm::vec2(x, y);
}

glm::vec3 StageLoader::ReadVec3(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y,z;
    iss >> x >> y >> z;
    return glm::vec3(x, y, z);
}

glm::quat StageLoader::ReadQuat(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return glm::quat(x, y, z, w);
}

std::string StageLoader::ReadString(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    std::string str;
    iss >> str;
    return str;
}

std::string StageLoader::ReadTag(std::string &line)
{
    return line.substr(1, line.find('>')-1);
}

Transform *StageLoader::ReadTransform(std::ifstream &f)
{
    Transform *t = new Transform();
    t->position = ReadVec3(f);
    t->rotation = ReadQuat(f);
    t->scale    = ReadVec3(f);
    return t;
}

Entity *StageLoader::ReadEntity(std::ifstream &f)
{
    Entity *e = new Entity();

    std::string line;

    //Read name
    line = GetLine(f);
    e->SetName(line);

    while( (line = GetLine(f)) != "")
    {
        if(line.at(0) == '<')
        {
            std::string tag = ReadTag(line);
            if(tag == "children")
            {
                //Read children
                while( (line = GetLine(f)) != "" )
                {
                    if(line != "</children>")
                    {
                        Entity *child = ReadEntity(f);
                        e->AddChild(child);
                    }
                    else break;
                }
            }
            else if(tag == "parts")
            {
                //Read parts
                while( (line = GetLine(f)) != "" )
                {
                    if(line != "</parts>" )
                    {
                        if(line.at(0) == '<')
                        {
                            std::string tag = ReadTag(line);
                            if(tag.at(0) != '/')
                            {
                                Part *p = nullptr;
                                if(tag == "Transform")
                                {
                                    p = ReadTransform(f);
                                }
                                else if(tag == "Behaviour")
                                {
                                    //p = ReadBehaviour(f);
                                }

                                if(p != nullptr)
                                {
                                    e->AddPart(p);
                                }
                            }
                            else
                            {

                            }
                        }
                    }
                    else break;
                }
            }
        }
        else
        {

        }
    }
}


void StageLoader::LoadStage(const std::string &filepath, Stage* stage)
{
    stage = new Stage();
    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the stage.");
    }
    else
    {
        std::string line;
        while( (line = GetLine(f)) != "")
        {
            if(line.at(0) == '<')
            {
                std::string tag = ReadTag(line);
                if(tag.at(0) != '/')
                {
                    if(tag == "children")
                    {
                        //Read children
                        while( (line = GetLine(f)) != "" )
                        {
                            if(line != "</children>")
                            {
                                Entity *e = ReadEntity(f);
                                stage->AddChild(e);
                            }
                            else break;
                        }
                    }
                }
                else
                {

                }
            }
        }
    }
}

void StageLoader::SaveStage(const std::string &filepath, const Stage *stage)
{

}

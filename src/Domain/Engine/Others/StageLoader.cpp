#include "StageLoader.h"

std::map<std::string, void*> StageLoader::idToPointers;

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
    do
    {
        std::getline(f, line);
        TrimStringLeft(&line);
    }
    while( line.empty() || line.at(0) == '#'); //Skip all empty/comment lines

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

Rect StageLoader::ReadRect(std::ifstream &f)
{
    glm::quat q = ReadQuat(f);
    return Rect(q.x, q.y, q.z, q.w);
}

std::string StageLoader::ReadString(std::ifstream &f)
{
    std::istringstream iss(GetLine(f));
    std::string str;
    iss >> str;
    return str;
}

void StageLoader::RegisterNextPointer(std::ifstream &f, void *pointer)
{
    idToPointers[ ReadString(f) ] = pointer;
}

void StageLoader::ReadParts(std::ifstream &f, Entity **e)
{
    std::string line;
    while( (line = GetLine(f)) != "</parts>" )
    {
        Part *p = nullptr;
        if(line == "<Transform>")
        {
            p = ReadTransform(f);
        }
        else if(line == "<Behaviour>")
        {
            //p = ReadBehaviour(f);
        }
        else if(line == "<MeshPyramid>")
        {
            p = ReadMeshPyramid(f);
        }
        else if(line == "<MeshRenderer>")
        {
            p = ReadMeshRenderer(f);
        }
        else if(line == "<Camera>")
        {
            p = ReadCamera(f);
        }


        if(p != nullptr)
        {
            (*e)->AddPart(p);
        }
    }
}

Transform *StageLoader::ReadTransform(std::ifstream &f)
{
    Transform *t = new Transform();
    RegisterNextPointer(f, t);

    t->position = ReadVec3(f);
    t->rotation = ReadQuat(f);
    t->scale    = ReadVec3(f);
    GetLine(f); //Consume close tag
    return t;
}

Material *StageLoader::ReadMaterial(std::ifstream &f)
{
    Material *m = new Material();
    RegisterNextPointer(f, m);
    m->SetShaderProgram(new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv,
                                          ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv));
    Texture2D *tex = new Texture2D("res/testTexture.png");
    tex->SetTextureSlot(0);
    m->SetTexture(tex);
    GetLine(f); //Consume close tag
    return m;
}

void StageLoader::ReadChildren(std::ifstream &f, Entity **e)
{
    std::string line;
    while( (line = GetLine(f)) != "</children>" )
    {
        Entity *child = ReadEntity(f);
        (*e)->AddChild(child);
    }
}

MeshPyramid *StageLoader::ReadMeshPyramid(std::ifstream &f)
{
    MeshPyramid *mp = new MeshPyramid();
    RegisterNextPointer(f, mp);
    GetLine(f); //Consume close tag
    return mp;
}

MeshRenderer *StageLoader::ReadMeshRenderer(std::ifstream &f)
{
    MeshRenderer *mr = new MeshRenderer();
    RegisterNextPointer(f, mr);
    mr->SetMesh( ReadNextPointer<Mesh>(f) );
    mr->SetMaterial( ReadNextPointer<Material>(f) );
    GetLine(f); //Consume close tag
    return mr;
}

Camera *StageLoader::ReadCamera(std::ifstream &f)
{
    Camera *cam = new Camera();
    RegisterNextPointer(f, cam);
    cam->SetFovDegrees( ReadFloat(f) );
    cam->SetZNear( ReadFloat(f) );
    cam->SetZFar( ReadFloat(f) );
    cam->SetProjectionMode( ReadString(f) == "Perspective" ?
                                Camera::ProjectionMode::Perspective :
                                Camera::ProjectionMode::Orthographic);
    cam->SetOrthoRect( ReadRect(f) );
    GetLine(f); //Consume close tag
    return cam;
}

void StageLoader::ReadAssets(std::ifstream &f)
{
    std::string line;
    while( (line = GetLine(f)) != "</assets>" )
    {
        if(line == "<Material>")
        {
            ReadMaterial(f);
        }
    }
}

Entity *StageLoader::ReadEntity(std::ifstream &f)
{
    Entity *e = new Entity();

    RegisterNextPointer(f, e); //Read Entity id
    e->SetName( GetLine(f) );  //Read Entity name

    std::string line;
    while( (line = GetLine(f)) != "</Entity>")
    {
        if(line == "<children>")
        {
            ReadChildren(f, &e);
        }
        else if(line == "<parts>")
        {
            ReadParts(f, &e);
        }
        else
        {

        }
    }
    return e;
}


void StageLoader::LoadStage(const std::string &filepath, Stage** stage)
{
    idToPointers.clear();

    *stage = new Stage();
    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the stage.");
    }
    else
    {
        std::string line = GetLine(f); // Skip <Stage> line
        RegisterNextPointer(f, (*stage)); // Read Stage id
        (*stage)->SetName( ReadString(f) ); //Read Stage name

        while( (line = GetLine(f)) != "</Stage>")
        {
            if(line == "") continue; //Skip blank lines

            if(line == "<assets>")
            {
                ReadAssets(f);
            }
            else if(line == "<children>")
            {
                ReadChildren(f, (Entity**)stage);
            }
            else
            {

            }
        }
    }
}

void StageLoader::SaveStage(const std::string &filepath, const Stage *stage)
{

}

#include "Mesh.h"
#include "Scene.h"
#include "Camera.h"
#include "Logger.h"
#include "stb_image.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "FileReader.h"
#include "GameObject.h"
#include "BehaviourHolder.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

//#include "BP_SceneReader_cpp_UserBehaviours_elseifs.bp"

const std::string FileReader::NoRegisterId = "-";
std::map<std::string, void*> FileReader::idToPointers;
void *FileReader::lastIstreamDir = nullptr;


unsigned char* FileReader::ReadImage(const std::string& filepath,
                                     int *width, int *height, int *components)
{
    unsigned char* data = stbi_load(filepath.c_str(),
                                    width, height,
                                    components, 0);
    if (!data)
    {
        Logger_Error("Error loading the texture '" << filepath <<
                     "', couldn't open/read the file.");
        return data;
    }
    return data;
}

void FileReader::GetOBJFormat(const std::string& filepath, bool *hasUvs,
                              bool *hasNormals, bool *isTriangles)
{
    std::FILE *f;
    f = fopen(filepath.c_str(), "r");
    if (!f)
    {
        Logger_Error("Error trying to open '" << filepath << "'");
        return;
    }

    fseek(f, -3, SEEK_END);
    int n = 1;

    char c, lastChar;
    while (ftell(f) > 0)
    {
        lastChar = fgetc(f);
        c = fgetc(f);
        if ((lastChar == '\n' || lastChar == '\r') && c == 'f')
        {
            int foo;
            while (fgetc(f) == ' '); // Read spaces after 'f'
            fseek(f, -1, SEEK_CUR);
            foo = fscanf(f, "%d", &foo); // Read first index
             // Single index without slashes
            if (fgetc(f) == ' ') *hasUvs = *hasNormals = false;
            else //  Something like  5/*
            {
                *hasUvs = (fgetc(f) != '/');
                if (!*hasUvs) *hasNormals = true; // Something like 5//8
                if (*hasUvs) // Something like 5/8*
                {
                    fseek(f, -1, SEEK_CUR);
                    foo = fscanf(f, "%d", &foo); // Read second index
                    if (fgetc(f) == '/') // Something like 5/8/11
                    {
                        fseek(f, -1, SEEK_CUR);
                        foo = fscanf(f, "%d", &foo); // Read last index
                        *hasNormals = true;
                    }
                    else *hasNormals = false;
                }
            }

            // Are they tris or quads?
            lastChar = c;
            while (!feof(f) && (c = fgetc(f)) != '\n')
            {
                if (lastChar == ' ' && c != ' ') ++n;
                lastChar = c;
            }
            *isTriangles = (n <= 3);
            break;
        }
        fseek(f, -3, SEEK_CUR);
    }
    fclose(f);

    *isTriangles = true;
}

int FileReader::GetOBJNumFaces(const std::string &filepath)
{
    std::ifstream f(filepath, std::ios::in);
    if (!f.is_open())
        Logger_Error("Error opening the mesh file '" << filepath << "'");

    int numFaces = 0;
    std::string line;
    while (std::getline(f, line))
    {
        if (line.length() > 0 && line.at(0) == 'f')
        {
            ++numFaces;
        }
    }

    return numFaces;
}

bool FileReader::ReadOBJ(const std::string& filepath,
                         std::vector<Vector3> *vertexPos,
                         std::vector<Vector3> *vertexNormals,
                         std::vector<glm::vec2> *vertexUvs,
                         bool *isTriangles)
{
    std::vector<Vector3> disorderedVertexPos, disorderedVertexNormals;
    std::vector<glm::vec2> disorderedVertexUvs;
    std::vector<unsigned int> vertexPosIndexes,
                              vertexUvsIndexes,
                              vertexNormIndexes;
    bool hasUvs, hasNormals;

    GetOBJFormat(filepath, &hasUvs, &hasNormals, isTriangles);

    std::ifstream f(filepath, std::ios::in);
    if (!f.is_open())
        Logger_Error("Error opening the mesh file '" << filepath << "'");
    std::string line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string lineHeader;
        if (!(ss >> lineHeader)) continue;
        if (lineHeader == "v")
        {
            Vector3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            disorderedVertexPos.push_back(pos);
        }
        else if (hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            disorderedVertexUvs.push_back(uv);
        }
        else if (hasNormals && lineHeader == "vn") //Cargamos normals
        {
            Vector3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            disorderedVertexNormals.push_back(normal);
        }
        else if (lineHeader == "f")
        {
            unsigned int posIndices[4];
            unsigned int uvIndices[4];
            unsigned int normalIndices[4];

            for (int i = 0; i < 3; ++i)
            {
                ss >> posIndices[i];
                if (hasUvs)
                {
                    while(ss.peek() == '/') ss.ignore();  //Read the '/'s
                    ss >> uvIndices[i];
                }

                if (hasNormals)
                {
                    while(ss.peek() == '/') ss.ignore();
                    ss >> normalIndices[i];
                }
            }

            //Vertices 0,1 same for tris and quads in CCW
            for (int j = 0; j <= 1; ++j)
            {
                vertexPosIndexes.push_back(posIndices[j]);
                vertexUvsIndexes.push_back(uvIndices[j]);
                vertexNormIndexes.push_back(normalIndices[j]);
            }

            bool theresAFaceLeft = false;
            while(ss.peek() == '\n' || ss.peek() == '\r' || ss.peek() == ' ')
                ss.ignore();
            theresAFaceLeft = (ss.peek() != EOF);

            if (theresAFaceLeft)
            {
                //QUAD FOUND, turn it into two triangles
                //Finish first triangle in CCW
                ss >> posIndices[3];
                if (hasUvs)
                {
                    while(ss.peek() == '/') ss.ignore();
                    ss >> uvIndices[3];
                }
                if (hasNormals)
                {
                    while(ss.peek() == '/') ss.ignore();
                    ss >> normalIndices[3];
                }
                vertexPosIndexes.push_back(posIndices[3]);
                vertexUvsIndexes.push_back(uvIndices[3]);
                vertexNormIndexes.push_back(normalIndices[3]);

                //Make second triangle in CCW
                for (int j = 1; j <= 3; ++j) //3,2,1
                {
                    vertexPosIndexes.push_back(posIndices[j]);
                    vertexUvsIndexes.push_back(uvIndices[j]);
                    vertexNormIndexes.push_back(normalIndices[j]);
                }
            }
            else //Triangles, finish with index 2
            {
                vertexPosIndexes.push_back(posIndices[2]);
                vertexUvsIndexes.push_back(uvIndices[2]);
                vertexNormIndexes.push_back(normalIndices[2]);
            }
        }
    }

    for (unsigned int i = 0; i < vertexPosIndexes.size(); ++i)
    {
        vertexPos->push_back(disorderedVertexPos[vertexPosIndexes[i]-1]);
    }

    if (hasNormals)
    {
        for (unsigned int i = 0; i < vertexNormIndexes.size(); ++i)
        {
            vertexNormals->push_back(
                        disorderedVertexNormals[vertexNormIndexes[i]-1]);
        }
    }

    if (hasUvs)
    {
        for (unsigned int i = 0; i < vertexUvsIndexes.size(); ++i)
        {
            vertexUvs->push_back(disorderedVertexUvs[vertexUvsIndexes[i]-1]);
        }
    }

    return true;
}


void FileReader::ReadComponents(std::istream &f, GameObject *e)
{
    std::string line;
    while ( (line = FileReader::ReadNextLine(f)) != "</components>" )
    {
        Component *p = nullptr;
        bool addComp = true;

        if (line == "<Transform>")
        {
            e->transform->Read(f); // Read on top of existing default Transform
            p = e->transform;
            addComp = false;
        }
        else if (line == "<MeshRenderer>")
        {
            MeshRenderer *mr = new MeshRenderer();
            mr->Read(f);
            p = mr;
        }
        else if (line == "<Camera>")
        {
            Camera *cam = new Camera();
            cam->Read(f);
            p = cam;
        }
        else if (line == "<BehaviourHolder>")
        {
            BehaviourHolder *bh = new BehaviourHolder();
            bh->Read(f);
            p = bh;
        }
        else
        {
            //BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS();

            /*
            BANG_PREPROCESSOR
            Here the BangPreprocessor with the macro BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS
            will write something like this:
                 if (line == "<UserBehaviour1>")
                 {
                      p = new UserBehaviour1();
                      ReadNextLine(f);
                 }
                 else if (line == "<UserBehaviour2>")
                 {
                      p = new UserBehaviour2();
                      ReadNextLine(f);
                 }
                 ...
            In order to complete the rest of the else if, for the custom user behaviours!!!
            */
        }

        if (p && addComp)
        {
            e->AddComponent(p);
        }
    }
}

void FileReader::ReadChildren(std::istream &f, GameObject *e)
{
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</children>")
    {
        if (line == "<GameObject>")
        {
            GameObject *child = new GameObject();
            child->Read(f);
            child->SetParent(e);
        }
        else if (line == "<GameObjectPrefab>")
        {
            std::string prefabFilepath = FileReader::ReadString(f);
            Prefab *p = AssetsManager::GetAsset<Prefab>(prefabFilepath);
            GameObject *child = p->Instantiate();
            child->SetParent(e);
        }
    }
}

void FileReader::ReadScene(const std::string &filepath, Scene* scene)
{
    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the scene.");
    }
    else
    {
        std::string line = FileReader::ReadNextLine(f); // Skip <Scene> line
        RegisterNextPointerId(f, scene); // Read Scene id
        scene->SetName( FileReader::ReadString(f) ); //Read Scene name

        while( (line = FileReader::ReadNextLine(f)) != "</Scene>")
        {
            if (line == "") continue; //Skip blank lines

            if (line == "<children>")
            {
                ReadChildren(f, (GameObject*) scene);
            }
            else if (line == "<cameraGameObject>")
            {
                GameObject *camChild = GetNextPointerAddress<GameObject>(f);
                if (camChild )
                {
                    scene->SetCamera(camChild->GetComponent<Camera>());
                }
                FileReader::ReadString(f);
            }
            else
            {
            }
        }
    }
}

void FileReader::SaveScene(const std::string &filepath, const Scene *scene)
{

}


void FileReader::TrimStringLeft(std::string *str)
{
    unsigned int i = 0;
    for (; i < str->length(); ++i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(i, str->length() - i);
}

void FileReader::GetImageFormat(const std::string &filepath, int *width,
                                int *height, int *numComponents)
{
    stbi_info(filepath.c_str(), width, height, numComponents);
}

std::string FileReader::ReadNextLine(std::istream &f)
{
    std::string line;
    do
    {
        std::getline(f, line);
        TrimStringLeft(&line);
    }
    while( (line.empty() || line.at(0) == '#' || line.at(0) == '\n') &&
           f.peek() != EOF); //Skip all empty/comment lines

    return line;
}

bool FileReader::ReadNextLine(std::istream &f, std::string *line)
{
    if (f.peek() == EOF) return false;

    do
    {
        std::getline(f, *line);
        TrimStringLeft(line);
    }
    while( (line->empty() || line->at(0) == '#' || line->at(0) == '\n') &&
           f.peek() != EOF); //Skip all empty/comment lines

    return true;
}

float FileReader::ReadFloat(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float v; iss >> v; return v;
}

glm::vec2 FileReader::ReadVec2(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y;
    iss >> x >> y;
    return glm::vec2(x, y);
}

Vector3 FileReader::ReadVec3(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z;
    iss >> x >> y >> z;
    return Vector3(x, y, z);
}

glm::vec4 FileReader::ReadVec4(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return glm::vec4(x, y, z, w);
}

Quaternion FileReader::ReadQuat(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return Quaternion(w, x, y, z);
}

Rect FileReader::ReadRect(std::istream &f)
{
    //Left, right, bottom, top
    Quaternion q = ReadQuat(f);
    return Rect(q.x, q.y, q.z, q.w);
}

std::string FileReader::ReadString(std::istream &f)
{
    std::string str;
    std::getline(f, str);
    TrimStringLeft(&str);
    return str;
}

void FileReader::RegisterNextPointerId(std::istream &f, void *pointer)
{

    if (&f != lastIstreamDir)
    {
        //We are reading a new file!
        idToPointers.clear();
    }
    lastIstreamDir = &f;

    std::string id = ReadString(f);
    if (id != NoRegisterId)
        idToPointers[id] = pointer;
}

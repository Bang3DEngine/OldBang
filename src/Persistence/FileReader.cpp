#include "Mesh.h"
#include "Scene.h"
#include "Camera.h"
#include "Debug.h"
#include "stb_image.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "FileReader.h"
#include "GameObject.h"
#include "PointLight.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#endif

const String FileReader::NoRegisterId = "-";
void *FileReader::lastIstreamDir = nullptr;


unsigned char* FileReader::ReadImage(const String& filepath,
                                     int *width, int *height)
{
    int comps;
    unsigned char* data = stbi_load(filepath.ToCString(),
                                    width, height, &comps, 4);
    if (!data)
    {
        Debug_Error("Error loading the texture '" << filepath <<
                     "', couldn't open/read the file.");
    }
    return data;
}

void FileReader::GetOBJFormat(const String& filepath, bool *hasUvs,
                              bool *hasNormals, bool *isTriangles)
{
    std::FILE *f;
    f = fopen(filepath.ToCString(), "r");
    if (!f)
    {
        Debug_Error("Error trying to open '" << filepath << "'");
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

int FileReader::GetOBJNumFaces(const String &filepath)
{
    std::ifstream f(filepath, std::ios::in);
    if (!f.is_open())
        Debug_Error("Error opening the mesh file '" << filepath << "'");

    int numFaces = 0;
    String line;
    while (std::getline(f, line))
    {
        if (!line.Empty() && line.At(0) == 'f')
        {
            ++numFaces;
        }
    }

    return numFaces;
}

bool FileReader::ReadOBJ(const String& filepath,
                         std::vector<Vector3> *vertexPos,
                         std::vector<Vector3> *vertexNormals,
                         std::vector<Vector2> *vertexUvs,
                         bool *isTriangles)
{
    std::vector<Vector3> disorderedVertexPos, disorderedVertexNormals;
    std::vector<Vector2> disorderedVertexUvs;
    std::vector<unsigned int> vertexPosIndexes,
                              vertexUvsIndexes,
                              vertexNormIndexes;
    bool hasUvs, hasNormals;

    GetOBJFormat(filepath, &hasUvs, &hasNormals, isTriangles);

    std::ifstream f(filepath, std::ios::in);
    if (!f.is_open())
        Debug_Error("Error opening the mesh file '" << filepath << "'");
    String line;

    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        String lineHeader;
        if (!(ss >> lineHeader)) continue;
        if (lineHeader == "v")
        {
            Vector3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            disorderedVertexPos.push_back(pos);
        }
        else if (hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            Vector2 uv;
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
                    while (ss.peek() == '/') ss.ignore();  //Read the '/'s
                    ss >> uvIndices[i];
                }

                if (hasNormals)
                {
                    while (ss.peek() == '/') ss.ignore();
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
            while (ss.peek() == '\n' || ss.peek() == '\r' || ss.peek() == ' ')
                ss.ignore();
            theresAFaceLeft = (ss.peek() != EOF);

            if (theresAFaceLeft)
            {
                //QUAD FOUND, turn it into two triangles
                //Finish first triangle in CCW
                ss >> posIndices[3];
                if (hasUvs)
                {
                    while (ss.peek() == '/') ss.ignore();
                    ss >> uvIndices[3];
                }
                if (hasNormals)
                {
                    while (ss.peek() == '/') ss.ignore();
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

void FileReader::ReadScene(const String &filepath, Scene* scene)
{
    XMLNode *xmlInfo = XMLParser::FromFile(filepath);
    if (xmlInfo)
    {
        scene->ReadXMLInfo(xmlInfo);
    }
    scene->PostReadXMLInfo(xmlInfo);
}

String FileReader::PeekNextLine(std::istream &f)
{
    std::streampos beginningOfLine = f.tellg();
    String line;
    do
    {
        beginningOfLine = f.tellg();
        std::getline(f, line);
        StringUtils::TrimLeft(&line);
    }
    while ( (line.Empty() || line.at(0) == '#' || line.at(0) == '\n') &&
           f.peek() != EOF); //Skip all empty/comment lines

    f.seekg(beginningOfLine);
    return line;
}

String FileReader::GetContents(const String &filepath)
{
    if (filepath == "") return "";

    std::fstream f;
    f.open(filepath);
    if (f.is_open())
    {
        String contents((std::istreambuf_iterator<char>(f)),
                              std::istreambuf_iterator<char>());
        return contents;
    }
    return "";
}

void FileReader::GetImageFormat(const String &filepath, int *width,
                                int *height, int *numComponents)
{
    stbi_info(filepath.ToCString(), width, height, numComponents);
}

String FileReader::ReadNextLine(std::istream &f)
{
    String line;
    do
    {
        std::getline(f, line);
        StringUtils::TrimLeft(&line);
    }
    while ( (line.Empty() || line.at(0) == '#' || line.at(0) == '\n') &&
           f.peek() != EOF); //Skip all empty/comment lines

    return line;
}

bool FileReader::ReadNextLine(std::istream &f, String *line)
{
    if (f.peek() == EOF) return false;

    do
    {
        std::getline(f, *line);
        StringUtils::TrimLeft(line);
    }
    while ( (line->empty() || line->at(0) == '#' || line->at(0) == '\n') &&
           f.peek() != EOF); //Skip all empty/comment lines

    return true;
}

bool FileReader::ReadBool(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    String b; iss >> b;  return (b == "true");
}

int FileReader::ReadInt(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    int v; iss >> v; return v;
}

float FileReader::ReadFloat(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float v; iss >> v; return v;
}

Vector2 FileReader::ReadVec2(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y;
    iss >> x >> y;
    return Vector2(x, y);
}

Vector3 FileReader::ReadVec3(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z;
    iss >> x >> y >> z;
    return Vector3(x, y, z);
}

Vector4 FileReader::ReadVec4(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return Vector4(x, y, z, w);
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

String FileReader::ReadString(std::istream &f)
{
    String str;
    std::getline(f, str);
    StringUtils::TrimLeft(&str);
    return str;
}

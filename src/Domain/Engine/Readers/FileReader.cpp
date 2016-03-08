#include "FileReader.h"
#include "stb_image.h"
#include "Stage.h"

unsigned char* FileReader::ReadImage(const std::string& filepath, int *components, int *width, int *height)
{
    unsigned char* data = stbi_load(filepath.c_str(), width, height, components, 0);
    if(data == nullptr)
    {
        Logger_Error("Error loading the texture '" << filepath << "', couldn't open/read the file.");
        return data;
    }
    return data;
}

void FileReader::GetOBJFormat(const std::string& filepath, bool *hasUvs, bool *hasNormals, bool *isTriangles)
{
    std::FILE *f;
    f = fopen(filepath.c_str(), "r");
    if(!f)
    {
        Logger_Error("Error trying to open '" << filepath << "'");
        return;
    }

    fseek(f, -3, SEEK_END);
    int n = 1;

    char c, lastChar;
    while(ftell(f) > 0)
    {
        lastChar = fgetc(f);
        c = fgetc(f);
        if((lastChar == '\n' || lastChar == '\r') && c == 'f')
        {
            int foo;
            while(fgetc(f) == ' '); //Leemos espacios despues de 'f'
            fseek(f, -1, SEEK_CUR);
            foo = fscanf(f, "%d", &foo); //Leemos primer indice
            if(fgetc(f) == ' ') *hasUvs = *hasNormals = false; //Solo un indice, sin barras
            else //Hay algo tal que asi:  5/*
            {
                *hasUvs = (fgetc(f) != '/');
                if(!*hasUvs) *hasNormals = true; //Es tal que asi 5//8
                if(*hasUvs) //Es algo tal que asi 5/8*
                {
                    fseek(f, -1, SEEK_CUR);
                    foo = fscanf(f, "%d", &foo); //Leemos segundo indice
                    if(fgetc(f) == '/') //Es algo tal que asi 5/8/11
                    {
                        fseek(f, -1, SEEK_CUR);
                        foo = fscanf(f, "%d", &foo); //Leemos ultimo indice
                        *hasNormals = true;
                    }
                    else *hasNormals = false;
                }
            }

            //Son triangulos o quads?
            lastChar = c;
            while(!feof(f) && (c = fgetc(f)) != '\n')
            {
                if(lastChar == ' ' && c != ' ') ++n;
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

bool FileReader::ReadOBJ(const std::string& filepath, std::vector<glm::vec3> *vertexPos,
                         std::vector<glm::vec3> *vertexNormals,
                         std::vector<glm::vec2> *vertexUvs,
                         bool *isTriangles)
{
    std::vector<glm::vec3> disorderedVertexPos, disorderedVertexNormals;
    std::vector<glm::vec2> disorderedVertexUvs;
    std::vector<unsigned int> vertexPosIndexes, vertexUvsIndexes, vertexNormIndexes;
    bool hasUvs, hasNormals;

    GetOBJFormat(filepath, &hasUvs, &hasNormals, isTriangles);

    std::ifstream f(filepath, std::ios::in);
    if(!f.is_open()) Logger_Error("Error opening the mesh file '" << filepath << "'");
    std::string line;

    while(std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string lineHeader;
        if(!(ss >> lineHeader)) continue;
        if(lineHeader == "v")
        {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            disorderedVertexPos.push_back(pos);
        }
        else if(hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            disorderedVertexUvs.push_back(uv);
        }
        else if(hasNormals && lineHeader == "vn") //Cargamos normals
        {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            disorderedVertexNormals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            unsigned int posIndices[4];
            unsigned int uvIndices[4];
            unsigned int normalIndices[4];

            for (int i = 0; i < 3; ++i)
            {
                ss >> posIndices[i];
                if(hasUvs)
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
            for(int j = 0; j <= 1; ++j)
            {
                vertexPosIndexes.push_back(posIndices[j]);
                vertexUvsIndexes.push_back(uvIndices[j]);
                vertexNormIndexes.push_back(normalIndices[j]);
            }

            bool theresAFaceLeft = false;
            while(ss.peek() == '\n' || ss.peek() == '\r' || ss.peek() == ' ')
                ss.ignore();
            theresAFaceLeft = (ss.peek() != EOF);

            if(theresAFaceLeft)
            {
                //QUAD FOUND, turn it into two triangles
                //Finish first triangle in CCW
                ss >> posIndices[3];
                if(hasUvs)
                {
                    while(ss.peek() == '/') ss.ignore();
                    ss >> uvIndices[3];
                }
                if(hasNormals)
                {
                    while(ss.peek() == '/') ss.ignore();
                    ss >> normalIndices[3];
                }
                vertexPosIndexes.push_back(posIndices[3]);
                vertexUvsIndexes.push_back(uvIndices[3]);
                vertexNormIndexes.push_back(normalIndices[3]);

                //Make second triangle in CCW
                for(int j = 1; j <= 3; ++j) //3,2,1
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

    for(unsigned int i = 0; i < vertexPosIndexes.size(); ++i)
    {
        vertexPos->push_back(disorderedVertexPos[vertexPosIndexes[i]-1]);
    }

    if(hasNormals)
    {
        for(unsigned int i = 0; i < vertexNormIndexes.size(); ++i)
        {
            vertexNormals->push_back(disorderedVertexNormals[vertexNormIndexes[i]-1]);
        }
    }

    if(hasUvs)
    {
        for(unsigned int i = 0; i < vertexUvsIndexes.size(); ++i)
        {
            vertexUvs->push_back(disorderedVertexUvs[vertexUvsIndexes[i]-1]);
        }
    }

    return true;
}

void FileReader::TrimStringLeft(std::string *str)
{
    unsigned int i = 0;
    for(; i < str->length(); ++i)
    {
        if(str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = str->substr(i, str->length() - i);
}

std::string FileReader::ReadNextLine(std::istream &f)
{
    std::string line;
    do
    {
        std::getline(f, line);
        TrimStringLeft(&line);
    }
    while( (line.empty() || line.at(0) == '#' || line.at(0) == '\n') && f.peek() != EOF); //Skip all empty/comment lines

    return line;
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

glm::vec3 FileReader::ReadVec3(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z;
    iss >> x >> y >> z;
    return glm::vec3(x, y, z);
}

glm::quat FileReader::ReadQuat(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    float x,y,z,w;
    iss >> x >> y >> z >> w;
    return glm::quat(x, y, z, w);
}

Rect FileReader::ReadRect(std::istream &f)
{
    glm::quat q = ReadQuat(f);
    return Rect(q.x, q.y, q.z, q.w);
}

std::string FileReader::ReadString(std::istream &f)
{
    std::istringstream iss(ReadNextLine(f));
    std::string str;
    iss >> str;
    return str;
}

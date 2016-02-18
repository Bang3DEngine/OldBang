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

    while(getline(f, line))
    {
        std::stringstream ss(line);
        std::string lineHeader;
        if(!(ss >> lineHeader)) continue;
        if(lineHeader == "v")
        {
            glm::vec3 pos;
            ss >> pos.x;
            ss >> pos.y;
            ss >> pos.z;
            disorderedVertexPos.push_back(pos);
        }
        else if(hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            glm::vec2 uv;
            ss >> uv.x;
            ss >> uv.y;
            disorderedVertexUvs.push_back(uv);
        }
        else if(hasNormals && lineHeader == "vn") //Cargamos normals
        {
            glm::vec3 normal;
            ss >> normal.x;
            ss >> normal.y;
            ss >> normal.z;
            disorderedVertexNormals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            int n = isTriangles ? 3 : 4;
            unsigned int index;
            char c;

            for (int i = 0; i < n; ++i)
            {
                ss >> index;
                vertexPosIndexes.push_back(index);

                if(hasUvs)
                {
                    ss >> c;  //Read the '/'
                    ss >> index;
                    vertexUvsIndexes.push_back(index);

                    if (hasNormals)
                    {
                        ss >> c;
                        ss >> index;
                        vertexNormIndexes.push_back(index);
                    }
                }
                else
                {
                    if (hasNormals)
                    {
                        ss >> c;
                        ss >> index;
                        vertexNormIndexes.push_back(index);
                    }
                }
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

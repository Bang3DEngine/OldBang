#include "Bang/FileReader.h"

#include "Bang/stb_image.h"

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/IO.h"

unsigned char* FileReader::ReadImage(const String& filepath,
                                     int *width, int *height)
{
    int comps;
    unsigned char* data = stbi_load(filepath.ToCString(),
                                    width, height, &comps, 4);
    if (!data)
    {
        Debug_Error("Error loading the image '" << filepath <<
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
                         Array<Vector3> *vertexPos,
                         Array<Vector3> *vertexNormals,
                         Array<Vector2> *vertexUvs,
                         bool *isTriangles)
{
    Array<Vector3> disorderedVertexPos, disorderedVertexNormals;
    Array<Vector2> disorderedVertexUvs;
    Array<unsigned int> vertexPosIndexes,
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
            disorderedVertexPos.PushBack(pos);
        }
        else if (hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            Vector2 uv;
            ss >> uv.x >> uv.y;
            disorderedVertexUvs.PushBack(uv);
        }
        else if (hasNormals && lineHeader == "vn") //Cargamos normals
        {
            Vector3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            disorderedVertexNormals.PushBack(normal);
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
                vertexPosIndexes.PushBack(posIndices[j]);
                vertexUvsIndexes.PushBack(uvIndices[j]);
                vertexNormIndexes.PushBack(normalIndices[j]);
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
                vertexPosIndexes.PushBack(posIndices[3]);
                vertexUvsIndexes.PushBack(uvIndices[3]);
                vertexNormIndexes.PushBack(normalIndices[3]);

                //Make second triangle in CCW
                for (int j = 1; j <= 3; ++j) //3,2,1
                {
                    vertexPosIndexes.PushBack(posIndices[j]);
                    vertexUvsIndexes.PushBack(uvIndices[j]);
                    vertexNormIndexes.PushBack(normalIndices[j]);
                }
            }
            else //Triangles, finish with index 2
            {
                vertexPosIndexes.PushBack(posIndices[2]);
                vertexUvsIndexes.PushBack(uvIndices[2]);
                vertexNormIndexes.PushBack(normalIndices[2]);
            }
        }
    }

    for (unsigned int i = 0; i < vertexPosIndexes.Size(); ++i)
    {
		const int vIndex = vertexPosIndexes[i] - 1;
		if (vIndex < disorderedVertexPos.Size())
		{
			vertexPos->PushBack(disorderedVertexPos[vIndex]);
		}
    }

    if (hasNormals)
    {
        for (unsigned int i = 0; i < vertexNormIndexes.Size(); ++i)
        {
			const int vIndex = vertexNormIndexes[i] - 1;
			if (vIndex < disorderedVertexNormals.Size())
			{
				vertexNormals->PushBack( disorderedVertexNormals[vIndex]);
			}
        }
    }

    if (hasUvs)
    {
		for (unsigned int i = 0; i < vertexUvsIndexes.Size(); ++i)
		{
			const int vIndex = vertexUvsIndexes[i] - 1;
			if (vIndex < disorderedVertexUvs.Size())
			{
				vertexUvs->PushBack(disorderedVertexUvs[vIndex]);
			}
		}
    }

    return true;
}

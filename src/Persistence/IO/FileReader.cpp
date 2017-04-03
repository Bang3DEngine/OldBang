#include "Bang/FileReader.h"

#include "Bang/IO.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"

void FileReader::GetOBJFormat(const String& filepath, bool *hasUvs,
                              bool *hasNormals, bool *isTriangles)
{
    Array<String> lines = IO::GetFileContents(filepath).Split('\n');
    for (const String &_line : lines)
    {
        String line = _line.Replace("\r", "");
        if (line.BeginsWith("f"))
        {
            while (line.Contains("  ")) { line = line.Replace("  ", " "); }
            Array<String> faceVertices = line.Split(' ');
            String samplePart = "";
            for (const String &part : faceVertices)
            {
                if (part.Contains("/")) { samplePart = part; break; }
            }

            if (!samplePart.Empty())
            {
                Array<String> vertexIndices = samplePart.Split('/');
                if (vertexIndices.Size() >= 3)
                {
                    *hasUvs     = !vertexIndices[1].Empty();
                    *hasNormals = !vertexIndices[2].Empty();
                    *isTriangles = faceVertices.Size() == 1 + 3; // f + ...
                    return;
                }
            }
        }
    }
}

int FileReader::GetOBJNumFaces(const String &filepath)
{
    int numFaces = 0;
    String contents = IO::GetFileContents(filepath);
    Array<String> lines = contents.Split('\n');
    for (const String& _line : lines)
    {
        String line = _line.Replace("\r", "");
        if (!line.Empty() && line.BeginsWith("f")) { ++numFaces; }
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

    String contents = IO::GetFileContents(filepath);
    Array<String> lines = contents.Split('\n');
    for (const String& _line : lines)
    {
        String line = _line.Replace("\r", "");

        std::stringstream ss(line);
        String lineHeader;
        if (!(ss >> lineHeader)) continue;
        if (lineHeader == "v")
        {
            Vector3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            disorderedVertexPos.PushBack(pos);
        }
        else if (hasUvs && lineHeader == "vt") // Load uvs
        {
            Vector2 uv;
            ss >> uv.x >> uv.y;
            disorderedVertexUvs.PushBack(uv);
        }
        else if (hasNormals && lineHeader == "vn") // Load normals
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
                while (ss.peek() == '/' || ss.peek() == ' ') ss.ignore();
                ss >> posIndices[i];
                if (hasUvs)
                {
                    while (ss.peek() == '/' || ss.peek() == ' ') ss.ignore();
                    ss >> uvIndices[i];
                }

                if (hasNormals)
                {
                    while (ss.peek() == '/' || ss.peek() == ' ') ss.ignore();
                    ss >> normalIndices[i];
                }
            }

            //Vertices 0,1 same for tris and quads in CCW
            for (int i = 0; i <= 1; ++i)
            {
                vertexPosIndexes.PushBack(posIndices[i]);
                vertexUvsIndexes.PushBack(uvIndices[i]);
                vertexNormIndexes.PushBack(normalIndices[i]);
            }

            while (ss.peek() == '\n' || ss.peek() == '\r' || ss.peek() == ' ')
                ss.ignore();

            const bool theresAVertexLeft = (ss.peek() != EOF);
            if (theresAVertexLeft)
            {
                //QUAD FOUND, turn it into two triangles
                //Finish first triangle in CCW
                ss >> posIndices[3];
                if (hasUvs)
                {
                    while (ss.peek() == ' ' || ss.peek() == '/') ss.ignore();
                    ss >> uvIndices[3];
                }
                if (hasNormals)
                {
                    while (ss.peek() == ' ' || ss.peek() == '/') ss.ignore();
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

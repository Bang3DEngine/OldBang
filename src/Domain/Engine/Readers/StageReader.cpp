#include "StageReader.h"

#include "Mesh.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Logger.h"
#include "Stage.h"

void StageReader::ReadParts(std::istream &f, Entity *e)
{
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</parts>" )
    {
        Part *p = nullptr;
        if(line == "<Transform>")
        {
            Transform *t = new Transform();
            t->Read(f);
            p = t;
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
        else
        {
            #BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS

            /*
            BANG_PREPROCESSOR
            Here the BangPreprocessor with the macro BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS
            will write something like this:
                 if(line == "<UserBehaviour1>")
                 {
                      *p = new UserBehaviour1();
                      ReadNextLine(f);
                 }
                 else if(line == "<UserBehaviour2>")
                 {
                      *p = new UserBehaviour2();
                      ReadNextLine(f);
                 }
                 ...
            In order to complete the rest of the else if, for the custom user behaviours!!!
            */
        }

        if(p != nullptr)
        {
            e->AddPart(p);
        }
    }
}

void StageReader::ReadChildren(std::istream &f, Entity *e)
{
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</children>" )
    {
        if(line == "<Entity>")
        {
            Entity *child = new Entity();
            child->Read(f);
            e->AddChild(child);
        }
        else if(line == "<EntityPrefab>")
        {
            std::string prefabFilepath = FileReader::ReadString(f);
            Prefab *p = AssetsManager::GetAsset<Prefab>(prefabFilepath);
            Entity *child = p->Instantiate();
            e->AddChild(child);
        }
    }
}

void StageReader::ReadStage(const std::string &filepath, Stage* stage)
{
    ClearPointerIds();

    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the stage.");
    }
    else
    {
        std::string line = FileReader::ReadNextLine(f); // Skip <Stage> line
        RegisterNextPointerId(f, stage); // Read Stage id
        stage->SetName( FileReader::ReadString(f) ); //Read Stage name

        while( (line = FileReader::ReadNextLine(f)) != "</Stage>")
        {
            if(line == "") continue; //Skip blank lines

            if(line == "<children>")
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

#include "SceneReader.h"

#include "Mesh.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Logger.h"
#include "Scene.h"

void SceneReader::ReadComponents(std::istream &f, GameObject *e)
{
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</components>" )
    {
        Logger_Log(line);
        Component *p = nullptr;
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
            BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS();

            /*
            BANG_PREPROCESSOR
            Here the BangPreprocessor with the macro BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS
            will write something like this:
                 if(line == "<UserBehaviour1>")
                 {
                      p = new UserBehaviour1();
                      ReadNextLine(f);
                 }
                 else if(line == "<UserBehaviour2>")
                 {
                      p = new UserBehaviour2();
                      ReadNextLine(f);
                 }
                 ...
            In order to complete the rest of the else if, for the custom user behaviours!!!
            */
        }

        if(p != nullptr)
        {
            e->AddComponent(p);
        }
    }
}

void SceneReader::ReadChildren(std::istream &f, GameObject *e)
{
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</children>")
    {
        if(line == "<GameObject>")
        {
            GameObject *child = new GameObject();
            child->Read(f);
            e->AddChild(child);
        }
        else if(line == "<GameObjectPrefab>")
        {
            std::string prefabFilepath = FileReader::ReadString(f);
            Prefab *p = AssetsManager::GetAsset<Prefab>(prefabFilepath);
            GameObject *child = p->Instantiate();
            e->AddChild(child);
        }
    }
}

void SceneReader::ReadScene(const std::string &filepath, Scene* scene)
{
    ClearPointerIds();

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
            if(line == "") continue; //Skip blank lines

            if(line == "<children>")
            {
                ReadChildren(f, (GameObject*)scene);
            }
            else if(line == "<cameraGameObject>")
            {
                GameObject *camChild = GetNextPointerAddress<GameObject>(f);
                if(camChild != nullptr)
                {
                    scene->SetCameraChild(camChild->GetName());
                }
                FileReader::ReadString(f);
            }
            else
            {
            }
        }
    }
}

void SceneReader::SaveScene(const std::string &filepath, const Scene *scene)
{

}

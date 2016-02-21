#include "TestBehaviour.h"

#include "ShaderContract.h"
#include "Transform.h"
#include "Material.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestBehaviour::TestBehaviour()
{
    time = 0.0f;
    randomAxis = glm::vec3( float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000);
    randomAxis = glm::normalize(randomAxis);
}

void TestBehaviour::OnStart()
{
    Material *m = new Material();

    m->SetShaderProgram(new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv,
                                          ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv));

    //GetOwner()->GetPart<MeshRenderer>()->SetMaterial(m);
    m->SetDiffuseColor(glm::vec4( float(rand()%2000-1000)/1000,
                                  float(rand()%2000-1000)/1000,
                                  float(rand()%2000-1000)/1000,
                                  0.2f));
    m->SetTexture(AssetsManager::GetAsset<Texture2D>("res/Assets/woodTexture.asset"));

    GetOwner()->GetPart<MeshRenderer>()->SetMaterial(m);
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetOwner()->GetPart<Transform>();
    t->SetRotation(glm::angleAxis(1.0f * Time::GetDeltaTime(), randomAxis) * t->GetRotation());
}

void TestBehaviour::OnDestroy()
{

}



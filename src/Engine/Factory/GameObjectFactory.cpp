#include "Bang/GameObjectFactory.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/UITree.h"
#include "Bang/Camera.h"
#include "Bang/UIList.h"
#include "Bang/UIMask.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UISlider.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIScrollBar.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/ObjectManager.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/DirectionalLight.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

GameObject *GameObjectFactory::CreateGameObject(bool addTransform)
{
    GameObject *go = GameObject::Create<GameObject>();
    if (addTransform && !go->HasComponent<Transform>())
    {
        go->AddComponent<Transform>();
    }
    return go;
}

GameObject *GameObjectFactory::CreateUIGameObject(bool addComponents)
{
    GameObject *go = GameObject::Create<GameObject>();
    GameObjectFactory::CreateUIGameObjectInto(go, addComponents);
    return go;
}

GameObject *GameObjectFactory::CreateGameObjectNamed(const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);
    return go;
}

GameObject *GameObjectFactory::CreateUIGameObjectNamed(const String &name)
{
    GameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->SetName(name);
    return go;
}

void GameObjectFactory::CreateUIGameObjectInto(GameObject *go,  bool addComps)
{
    if (addComps)
    {
        if (!go->HasComponent<RectTransform>())
        { go->AddComponent<RectTransform>(); }
    }
}

Scene *GameObjectFactory::CreateScene(bool addTransform)
{
    Scene *scene = GameObject::Create<Scene>();
    if (addTransform && !scene->HasComponent<Transform>())
    {
        scene->AddComponent<Transform>();
    }
    return scene;
}

Scene *GameObjectFactory::CreateUIScene()
{
    Scene *scene = GameObject::Create<Scene>();
    CreateUISceneInto(scene);
    return scene;
}

void GameObjectFactory::CreateUISceneInto(Scene *scene)
{
    scene->AddComponent<Camera>();
    GameObjectFactory::CreateUIGameObjectInto(scene);
    GameObjectFactory::CreateUICanvasInto(scene);
}

class Rotator : public Component
{
public:
    void OnUpdate() override
    {
        Component::OnUpdate();
        GetGameObject()->GetTransform()->RotateLocal(Quaternion::AngleAxis(0.1f,Vector3::Up));
    }
};
class Scalator : public Component
{
public:
    void OnUpdate() override
    {
        Component::OnUpdate();
        GetGameObject()->GetTransform()->SetLocalScale(
                    Math::Abs( Math::Sin(Time::GetNow_Seconds()) ) + 1.0f );
    }
};

Scene *GameObjectFactory::CreateDefaultSceneInto(Scene *scene)
{
    GameObject *cube = GameObjectFactory::CreateGameObjectNamed("Cube");
    MeshRenderer *mr = cube->AddComponent<MeshRenderer>();
    cube->AddComponent<Rotator>();
    mr->SetMesh(MeshFactory::GetCube().Get());

    RH<Texture2D> texture =
            Resources::Load<Texture2D>(Paths::EngineAssets()
                                       .Append("Images/LogoBang_B_512.png"));
    mr->GetMaterial()->SetTexture(texture.Get());
    mr->GetMaterial()->SetDiffuseColor(Color::White);

    GameObject *sphere = GameObjectFactory::CreateGameObjectNamed("Sphere-Child");
    sphere->GetTransform()->SetLocalPosition(Vector3(1,1,1));
    sphere->GetTransform()->SetLocalScale( Vector3(0.3f) );
    MeshRenderer *mr2 = sphere->AddComponent<MeshRenderer>();
    mr2->SetMesh(MeshFactory::GetSphere().Get());
    mr2->GetMaterial()->SetDiffuseColor(Color::Green);

    GameObject *cube2 = GameObjectFactory::CreateGameObjectNamed("Cube-Sphere-Child");
    cube2->GetTransform()->SetLocalPosition(Vector3(4,0,0));
    cube2->AddComponent<Scalator>();
    MeshRenderer *mr3 = cube2->AddComponent<MeshRenderer>();
    mr3->SetMesh(MeshFactory::GetCube().Get());
    mr3->GetMaterial()->SetDiffuseColor(Color::Purple);

    GameObject *lightGo = GameObjectFactory::CreateGameObjectNamed("Light");
    PointLight *pl = lightGo->AddComponent<PointLight>();
    pl->SetRange(20.0f);
    pl->SetColor(Color::Yellow);
    lightGo->GetTransform()->SetPosition( Vector3(7,4,-2) );
    lightGo->GetTransform()->LookAt( Vector3::Zero );

    GameObject *light2Go = GameObjectFactory::CreateGameObjectNamed("Light2");
    PointLight *pl2 = light2Go->AddComponent<PointLight>();
    pl2->SetRange(20.0f);
    pl2->SetColor(Color::Purple);
    light2Go->GetTransform()->SetPosition( Vector3(-7,4,-2) );
    light2Go->GetTransform()->LookAt( Vector3::Zero );

    GameObject *light3Go = GameObjectFactory::CreateGameObjectNamed("Light3");
    PointLight *pl3 = light3Go->AddComponent<PointLight>();
    pl3->SetRange(6.0f);
    pl3->SetColor(Color::White);
    light3Go->GetTransform()->SetPosition( Vector3(0, 4, 0) );
    light3Go->GetTransform()->LookAt( Vector3::Zero );

    GameObject *floor = GameObjectFactory::CreateCubeGameObject();
    floor->SetName("Floor");
    floor->GetTransform()->TranslateLocal( Vector3(0, -1, 0) );
    floor->GetTransform()->SetLocalScale( Vector3(10.0f, 0.2f, 10.0f));

    GameObject *wall1 = GameObjectFactory::CreateCubeGameObject();
    wall1->SetName("Wall1");
    wall1->GetTransform()->TranslateLocal( Vector3(-4, 3, 0) );
    wall1->GetTransform()->SetLocalScale( Vector3(0.2f, 10.0f, 10.0f));

    GameObject *wall2 = GameObjectFactory::CreateCubeGameObject();
    wall2->SetName("Wall2");
    wall2->GetTransform()->TranslateLocal( Vector3(0, 3, -4) );
    wall2->GetTransform()->SetLocalRotation( Quaternion::AngleAxis(Math::Pi/2, Vector3::Up) );
    wall2->GetTransform()->SetLocalScale( Vector3(0.2f, 10.0f, 10.0f));

    GameObject *cameraGo = GameObjectFactory::CreateGameObjectNamed("Camera");
    cameraGo->GetTransform()->SetPosition( Vector3(5,4,3) );
    cameraGo->GetTransform()->LookAt( Vector3::Zero );
    Camera *cam = cameraGo->AddComponent<Camera>();
    cam->SetClearColor(Color::LightBlue);
    scene->SetCamera(cam);

    for (int i = 0; i < 0; ++i)
    {
        GameObjectFactory::CreateGameObjectNamed("GO_" + String(i))->SetParent(scene);
    }

    lightGo->SetParent(scene);
    light2Go->SetParent(scene);
    light3Go->SetParent(scene);
    floor->SetParent(scene);
    wall1->SetParent(scene);
    wall2->SetParent(scene);
    cube->SetParent(scene);
    sphere->SetParent(cube);
    cube2->SetParent(sphere);
    cameraGo->SetParent(scene);
    return scene;
}

UICanvas *GameObjectFactory::CreateUICanvas()
{
    GameObject *go = GameObjectFactory::CreateUIGameObject();
    return GameObjectFactory::CreateUICanvasInto(go);
}

UICanvas* GameObjectFactory::CreateUICanvasInto(GameObject *go)
{
    UICanvas *canvas = go->AddComponent<UICanvas>();
    go->SetName("Canvas");
    return canvas;
}

UIImageRenderer *GameObjectFactory::CreateUIImage(const Color &color)
{
    GameObject *go = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *img = go->AddComponent<UIImageRenderer>();
    img->SetTint(color);
    go->SetName("Image");
    return img;
}

UIImageRenderer *GameObjectFactory::CreateUIImage(const Color &color,
                                                  const Vector2i &size)
{
    UIImageRenderer *img = GameObjectFactory::CreateUIImage(color);
    UILayoutElement *le = img->GetGameObject()->AddComponent<UILayoutElement>();
    le->SetMinSize(size);
    le->SetPreferredSize(size);
    return img;
}

UIList *GameObjectFactory::CreateUIListInto(GameObject *go,
                                            bool withScrollPanel)
{
    return UIList::CreateInto(go, withScrollPanel);
}
UIList *GameObjectFactory::CreateUIList(bool withScrollPanel)
{
    return UIList::CreateInto( GameObjectFactory::CreateUIGameObjectNamed("List"),
                               withScrollPanel );
}

UITree *GameObjectFactory::CreateUITreeInto(GameObject *go)
{
    return UITree::CreateInto(go);
}

UITree *GameObjectFactory::CreateUITree()
{
    return UITree::CreateInto( GameObjectFactory::CreateUIGameObjectNamed("Tree") );
}

UIInputText *GameObjectFactory::CreateUIInputTextInto(GameObject *go)
{
    return UIInputText::CreateInto(go);
}
UIInputText *GameObjectFactory::CreateUIInputText()
{
    return GameObjectFactory::CreateUIInputTextInto(
                GameObjectFactory::CreateUIGameObjectNamed("InputText") );
}

UICheckBox *GameObjectFactory::CreateUICheckBoxInto(GameObject *go)
{
    return UICheckBox::CreateInto(go);
}

UICheckBox *GameObjectFactory::CreateUICheckBox()
{
    return GameObjectFactory::CreateUICheckBoxInto(
                GameObjectFactory::CreateUIGameObjectNamed("CheckBox") );
}

UIComboBox *GameObjectFactory::CreateUIComboBoxInto(GameObject *go)
{
    return UIComboBox::CreateInto(go);
}

UIComboBox *GameObjectFactory::CreateUIComboBox()
{
    return GameObjectFactory::CreateUIComboBoxInto(
                GameObjectFactory::CreateUIGameObjectNamed("ComboBox") );
}

UISlider *GameObjectFactory::CreateUISliderInto(GameObject *go)
{
    return UISlider::CreateInto(go);
}

UISlider *GameObjectFactory::CreateUISlider()
{
    return GameObjectFactory::CreateUISliderInto(
                GameObjectFactory::CreateUIGameObjectNamed("Slider") );
}

UIInputNumber *GameObjectFactory::CreateUIInputNumberInto(GameObject *go)
{
    return UIInputNumber::CreateInto(go);
}

UIInputNumber *GameObjectFactory::CreateUIInputNumber()
{
    return GameObjectFactory::CreateUIInputNumberInto(
                    GameObjectFactory::CreateUIGameObjectNamed("InputNumber") );
}

UIButton *GameObjectFactory::CreateUIButtonInto(GameObject *go)
{
    return UIButton::CreateInto(go);
}
UIButton* GameObjectFactory::CreateUIButton()
{
    return UIButton::CreateInto(
                GameObjectFactory::CreateUIGameObjectNamed("Button") );
}

UIButton *GameObjectFactory::CreateUIButton(const String &text,
                                                  Texture2D *icon)
{
    const Vector2i size(15);
    UIButton *btn = GameObjectFactory::CreateUIButton();

    if (!text.IsEmpty()) { btn->GetText()->SetContent(text); }

    if (icon) { btn->SetIcon(icon, size, 5); }

    return btn;
}

UILabel *GameObjectFactory::CreateUILabelInto(GameObject *go)
{
    return UILabel::CreateInto(go);
}
UILabel *GameObjectFactory::CreateUILabel()
{
    return UILabel::CreateInto( GameObjectFactory::CreateUIGameObjectNamed("Label") );
}

UIScrollArea *GameObjectFactory::CreateUIScrollArea()
{
    return UIScrollArea::CreateInto(
                    GameObjectFactory::CreateUIGameObjectNamed("ScrollArea") );
}

UIScrollBar *GameObjectFactory::CreateUIScrollBarInto(GameObject *go)
{
    return UIScrollBar::CreateInto(go);
}
UIScrollBar *GameObjectFactory::CreateUIScrollBar()
{
    return UIScrollBar::CreateInto(
                GameObjectFactory::CreateUIGameObjectNamed("ScrollBar") );
}

UIScrollPanel *GameObjectFactory::CreateUIScrollPanelInto(GameObject *go)
{
    return UIScrollPanel::CreateInto(go);
}
UIScrollPanel *GameObjectFactory::CreateUIScrollPanel()
{
    return GameObjectFactory::CreateUIScrollPanelInto(
                GameObjectFactory::CreateUIGameObjectNamed("ScrollPanel") );
}

UIScrollArea* GameObjectFactory::CreateUIScrollAreaInto(GameObject *go)
{
    return UIScrollArea::CreateInto(go);
}

GameObject *GameObjectFactory::CreateUISpacer(LayoutSizeType sizeType,
                                              const Vector2 &space)
{
    GameObject *spacerGo = GameObjectFactory::CreateUIGameObjectNamed("Separator");
    UILayoutElement *le = spacerGo->AddComponent<UILayoutElement>();

    le->SetMinSize( Vector2i(0) );
    le->SetPreferredSize( Vector2i(0) );
    le->SetFlexibleSize( Vector2(0) );

    if (sizeType == LayoutSizeType::Min) { le->SetMinSize(Vector2i(space)); }
    else if (sizeType == LayoutSizeType::Preferred) { le->SetPreferredSize(Vector2i(space)); }
    else { le->SetFlexibleSize( Vector2(space) ); }
    return spacerGo;
}
GameObject *GameObjectFactory::CreateUIHSpacer(LayoutSizeType sizeType,
                                               float spaceX)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateUISpacer(sizeType, Vector2(spaceX, 0) );
    return spacerGo;
}
GameObject *GameObjectFactory::CreateUIVSpacer(LayoutSizeType sizeType,
                                               float spaceY)
{
    GameObject *spacerGo =
            GameObjectFactory::CreateUISpacer(sizeType, Vector2(0, spaceY) );
    return spacerGo;
}

String GameObjectFactory::GetGameObjectDuplicateName(const GameObject *go)
{
    String originalName = go->GetName();

    String duplicateNameNumber = "";
    bool isDuplicatedName = false;
    for (int i = originalName.Size()-1; i >= 0; --i)
    {
        char c = originalName[i];
        if (String::IsNumber(c))
        {
            isDuplicatedName = true;
            duplicateNameNumber.Prepend( String(c) );
        }
        else
        {
            isDuplicatedName = (c == '_');
            break;
        }
    }


    String duplicateName;
    if (isDuplicatedName)
    {
        int duplicateNumber = String::ToInt(duplicateNameNumber);
        duplicateName = originalName.SubString(0, originalName.Size() -
                                                  duplicateNameNumber.Size() - 2);
        duplicateName += "_" + String(duplicateNumber+1);
    }
    else { duplicateName = (originalName + "_1"); }

    return duplicateName;
}

GameObject *GameObjectFactory::CreateUISeparator(LayoutSizeType sizeType,
                                                 const Vector2i &space,
                                                 float linePercent)
{
    GameObject *sepGo = GameObjectFactory::CreateUISpacer(sizeType,
                                                          Vector2(space));
    LineRenderer *lr = sepGo->AddComponent<LineRenderer>();
    lr->SetMaterial(MaterialFactory::GetUIImage().Get());
    lr->GetMaterial()->SetDiffuseColor(Color::White);
    lr->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    lr->SetRenderPass(RenderPass::Canvas);

    UILayoutElement *le = sepGo->GetComponent<UILayoutElement>();
    le->SetPreferredSize( Vector2i::Max(space, Vector2i::One) );
    bool horizontal = (space.x == 0);
    if (horizontal)
    {
        le->SetFlexibleSize( Vector2(99999999, 0) );
        lr->SetPoints( {Vector3(-linePercent,0,0), Vector3(linePercent,0,0)} );
    }
    else
    {
        le->SetFlexibleSize( Vector2(0, 99999999) );
        lr->SetPoints( {Vector3(0,-linePercent,0), Vector3(0,linePercent,0)} );
    }
    return sepGo;
}

GameObject *GameObjectFactory::CreateUIHSeparator(LayoutSizeType sizeType,
                                                  int spaceY,
                                                  float linePercent)
{
    GameObject *sepGo =
            GameObjectFactory::CreateUISeparator(sizeType, Vector2i(0, spaceY),
                                                 linePercent);
    return sepGo;
}
GameObject *GameObjectFactory::CreateUIVSeparator(LayoutSizeType sizeType,
                                                  int spaceX,
                                                  float linePercent)
{
    GameObject *sepGo =
            GameObjectFactory::CreateUISeparator(sizeType, Vector2i(spaceX, 0),
                                                 linePercent);
    return sepGo;
}

GameObject* GameObjectFactory::CreateGameObjectWithMesh(Mesh* m,
                                                        const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderPrimitive(GL::Primitive::Triangles);
    r->SetMaterial(MaterialFactory::GetDefault().Get());
    r->SetMesh(m);

    return go;
}

GameObject* GameObjectFactory::CreatePlaneGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetPlane();
    return CreateGameObjectWithMesh(mesh.Get(), "Plane");
}

GameObject* GameObjectFactory::CreateCubeGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetCube();
    return CreateGameObjectWithMesh(mesh.Get(), "Cube");
}

GameObject* GameObjectFactory::CreateSphereGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetSphere();
    return CreateGameObjectWithMesh(mesh.Get(), "Sphere");
}

GameObject* GameObjectFactory::CreateConeGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetCone();
    return CreateGameObjectWithMesh(mesh.Get(), "Cone");
}

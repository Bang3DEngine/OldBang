#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QMessageBox>

#include "Dialog.h"
#include "String.h"

class Mesh;
class GameObject;
class WindowEventManager;
class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    Q_ENUMS(Action);

    enum Action
    {
        NewScene,
        OpenScene,
        SaveScene,
        SaveSceneAs,
        Build,
        BuildAndRun,

        CreateEmptyGameObject,
        CreateEmptyUIGameObject,
        CreateFromPrefab,
        CreatePlane,
        CreateCube,
        CreateSphere,
        CreateCone,
        CreateDirectionalLight,
        CreatePointLight,
        CreateUIText,
        CreateUIImage,
        AlignGameObjectWithView,
        AlignViewWithGameObject,

        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,
        CreateFont,
        CreateAudioClip,

        AddComponentNewBehaviour,
        AddComponentBehaviour,
        AddComponentCamera,
        AddComponentMeshRenderer,
        AddComponenSingleLineRenderer,
        AddComponentCircleRenderer,
        AddComponentAudioSource,
        AddComponentAudioListener,
        AddComponentDirectionalLight,
        AddComponentPointLight,
        AddComponentCanvas,
        AddComponentUIImage,
        AddComponentUIText
    };

    MenuBar(QWidget *parent = nullptr);

    void CreateNewScene() const;
    Dialog::Reply AskForSavingActiveScene() const;

    static MenuBar *GetInstance();

public slots:

    void OnNewProject() const;
    void OnOpenProject() const;
    void OnSaveProject() const;
    void OnNewScene() const;
    void OnOpenScene() const;
    void OnSaveScene() const;
    void OnSaveSceneAs() const;
    void OnBuild() const;
    void OnBuildAndRun() const;

    void OnCreateEmptyGameObject() const;
    void OnCreateEmptyUIGameObject() const;
    void OnCreateFromPrefab() const;
    void OnCreatePlane() const;
    void OnCreateCube() const;
    void OnCreateSphere() const;
    void OnCreateCone() const;
    void OnCreateDirectionalLight() const;
    void OnCreatePointLight() const;
    void OnCreateUIText() const;
    void OnCreateUIImage() const;
    void OnAlignGameObjectWithView() const;
    void OnAlignViewWithGameObject() const;

    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;
    void OnCreateFont() const;
    void OnCreateAudioClip() const;

    void OnAddComponentNewBehaviour() const;
    void OnAddComponentBehaviour() const;
    void OnAddComponentCamera() const;
    void OnAddComponentMeshRenderer() const;
    void OnAddComponentSingleLineRenderer() const;
    void OnAddComponentCircleRenderer() const;
    void OnAddComponentAudioSource() const;
    void OnAddComponentAudioListener() const;
    void OnAddComponentDirectionalLight() const;
    void OnAddComponentPointLight() const;
    void OnAddComponentCanvas() const;
    void OnAddComponentUIImage() const;
    void OnAddComponentUIText() const;

private:
    static MenuBar *s_instance;
    WindowEventManager *m_wem = nullptr;

    /**
     * @brief Shared code to create a Plane, Cube, Sphere, etc.
     */
    GameObject* CreatePrimitiveGameObject(Mesh *m, const String &name) const;

};

#endif // MENUBAR_H

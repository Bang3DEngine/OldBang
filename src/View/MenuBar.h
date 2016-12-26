#ifndef MENUBAR_H
#define MENUBAR_H

#include "Bang.h"

#include <QMenuBar>
#include <QMessageBox>

#include "Mesh.h"
#include "GameBuilder.h"

class GameObject;
class WindowEventManager;
class MenuBar : public QMenuBar
{
    Q_OBJECT

private:
    WindowEventManager *m_wem = nullptr;

    /**
     * @brief Shared code to create a Plane, Cube, Sphere, etc.
     */
    GameObject* CreatePrimitiveGameObject(Mesh *m, const String &name) const;

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
        CreateFromPrefab,
        CreatePlane,
        CreateCube,
        CreateSphere,
        CreateCone,
        CreateDirectionalLight,
        CreatePointLight,
        AlignGameObjectWithView,
        AlignViewWithGameObject,

        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,
        CreateFont,

        AddComponentBehaviour,
        AddComponentCamera,
        AddComponentMeshRenderer,
        AddComponenSingleLineRenderer,
        AddComponentCircleRenderer,
        AddComponentDirectionalLight,
        AddComponentPointLight,
        AddComponentCanvas,
        AddComponentUIImage,
        AddComponentUIText
    };

    MenuBar(QWidget *parent = nullptr);

    void CreateNewScene () const;
    QMessageBox::StandardButton AskForSavingActiveScene () const;


public slots:

    void OnNewScene () const;
    void OnOpenScene () const;
    void OnSaveScene () const;
    void OnSaveSceneAs () const;
    void OnBuild () const;
    void OnBuildAndRun () const;

    void OnCreateEmptyGameObject () const;
    void OnCreateFromPrefab () const;
    void OnCreatePlane () const;
    void OnCreateCube () const;
    void OnCreateSphere () const;
    void OnCreateCone () const;
    void OnCreateDirectionalLight () const;
    void OnCreatePointLight () const;
    void OnAlignGameObjectWithView() const;
    void OnAlignViewWithGameObject() const;

    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;
    void OnCreateFont() const;

    void OnAddComponentBehaviour() const;
    void OnAddComponentCamera() const;
    void OnAddComponentMeshRenderer() const;
    void OnAddComponentSingleLineRenderer() const;
    void OnAddComponentCircleRenderer() const;
    void OnAddComponentDirectionalLight() const;
    void OnAddComponentPointLight() const;
    void OnAddComponentCanvas() const;
    void OnAddComponentUIImage() const;
    void OnAddComponentUIText() const;
};

#endif // MENUBAR_H

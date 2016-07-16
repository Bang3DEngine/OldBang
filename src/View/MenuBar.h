#ifndef MENUBAR_H
#define MENUBAR_H

#include "Bang.h"

#include <QMenuBar>
#include <QMessageBox>

#include "GameBuilder.h"
#include "Mesh.h"

class WindowEventManager;
class MenuBar : public QMenuBar
{
    Q_OBJECT

private:
    WindowEventManager *p_wem = nullptr;

    /**
     * @brief Shared code to create a Plane, Cube, Sphere, etc.
     */
    GameObject* CreatePrimitiveGameObject(Mesh *m) const;

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

        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,

        AddComponentBehaviour,
        AddComponentCamera,
        AddComponentTransform,
        AddComponentMeshRenderer,
        AddComponenSingleLineRenderer,
        AddComponentCircleRenderer
    };

    MenuBar(QWidget *parent = nullptr);

    void CreateNewScene () const;
    QMessageBox::StandardButton AskForSavingCurrentScene () const;


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

    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;

    void OnAddComponentBehaviour() const;
    void OnAddComponentCamera() const;
    void OnAddComponentTransform() const;
    void OnAddComponentMeshRenderer() const;
    void OnAddComponentSingleLineRenderer() const;
    void OnAddComponentCircleRenderer() const;
};

#endif // MENUBAR_H

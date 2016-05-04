#ifndef MENUBAR_H
#define MENUBAR_H

#include "Bang.h"

#include <QMenuBar>
#include <QMessageBox>

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

        CreateEmptyGameObject,
        CreateFromPrefab,

        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,

        AddComponentBehaviour,
        AddComponentCamera,
        AddComponentMeshRenderer,
        AddComponentLineRenderer,
        AddComponentTransform,
    };

    MenuBar(QWidget *parent = nullptr);

    void CreateNewScene () const;
    QMessageBox::StandardButton AskForSavingCurrentScene () const;

public slots:

    void OnNewScene () const;
    void OnOpenScene () const;
    void OnSaveScene () const;
    void OnSaveSceneAs () const;

    void OnCreateEmptyGameObject () const;
    void OnCreateFromPrefab () const;

    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;

    void OnAddComponentBehaviour() const;
    void OnAddComponentCamera() const;
    void OnAddComponentMeshRenderer() const;
    void OnAddComponentLineRenderer() const;
    void OnAddComponentTransform() const;
};

#endif // MENUBAR_H

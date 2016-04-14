#ifndef MENUBAR_H
#define MENUBAR_H

#include "Bang.h"

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    Q_ENUMS(Action);

    enum Action
    {
        OpenStage,
        SaveCurrentStage,

        CreateEmptyEntity,
        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,

        AddPartBehaviour,
        AddPartCamera,
        AddPartMeshRenderer,
        AddPartTransform,
    };

    MenuBar(QWidget *parent = nullptr);


public slots:

    void OnOpenStage () const;
    void OnSaveCurrentStage () const;

    void OnCreateEmptyEntity () const;
    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;

    void OnAddPartBehaviour() const;
    void OnAddPartCamera() const;
    void OnAddPartMeshRenderer() const;
    void OnAddPartTransform() const;
};

#endif // MENUBAR_H

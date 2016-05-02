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
        NewStage,
        OpenStage,
        SaveStage,
        SaveStageAs,

        CreateEmptyEntity,
        CreateFromPrefab,

        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D,

        AddPartBehaviour,
        AddPartCamera,
        AddPartMeshRenderer,
        AddPartLineRenderer,
        AddPartTransform,
    };

    MenuBar(QWidget *parent = nullptr);

    void CreateNewStage () const;
    QMessageBox::StandardButton AskForSavingCurrentStage () const;

public slots:

    void OnNewStage () const;
    void OnOpenStage () const;
    void OnSaveStage () const;
    void OnSaveStageAs () const;

    void OnCreateEmptyEntity () const;
    void OnCreateFromPrefab () const;

    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;

    void OnAddPartBehaviour() const;
    void OnAddPartCamera() const;
    void OnAddPartMeshRenderer() const;
    void OnAddPartLineRenderer() const;
    void OnAddPartTransform() const;
};

#endif // MENUBAR_H

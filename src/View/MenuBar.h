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
        CreateEmptyEntity,
        CreatePrefab,
        CreateMaterial,
        CreateMesh,
        CreateShaderProgram,
        CreateTexture2D
    };

    MenuBar(QWidget *parent = nullptr);


public slots:

    void OnCreateEmptyEntity () const;
    void OnCreatePrefab() const;
    void OnCreateMaterial() const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D() const;
};

#endif // MENUBAR_H

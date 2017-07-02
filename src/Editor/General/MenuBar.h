#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QMessageBox>
#include "Bang/WinUndef.h"

#include "Bang/Dialog.h"
#include "Bang/String.h"
#include "Bang/Camera.h"
#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/Behaviour.h"
#include "Bang/AudioClip.h"
#include "Bang/Transform.h"
#include "Bang/PointLight.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/RectTransform.h"
#include "Bang/AudioListener.h"
#include "Bang/CircleRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/SingleLineRenderer.h"

class Mesh;
class Material;
class AudioClip;
class Texture2D;
class WindowEventManager;
class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    Q_ENUMS(Action);

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
    void OpenScene(const Path &filepath) const;
    void OnSaveScene() const;
    void OnSaveSceneAs() const;
    void OnBuild() const;
    void OnBuildAndRun() const;

    void OnCreateEmptyGameObject() const;
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
    void OnCreateMaterial(const String &matName = "") const;
    void OnCreateMesh() const;
    void OnCreateShaderProgram() const;
    void OnCreateTexture2D(const String &tex2DName = "") const;
    void OnCreateFont() const;
    void OnCreateAudioClip(const String &audioClipName = "") const;

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
    void OnAddComponentPostProcessEffect() const;

public:
    void OnAddComponent(Component *c) const;

    template<class ComponentClass>
    void OnAddComponent() const
    {
        GameObject* inspectorGameObject = GetInspectorGameObject();
        ENSURE(inspectorGameObject);

        ComponentClass *c = inspectorGameObject->AddComponent<ComponentClass>();
        OnAddComponent(c);
    }

    GameObject* GetInspectorGameObject() const;

private:
    static MenuBar *s_instance;
    WindowEventManager *m_wem = nullptr;

    /**
     * @brief Shared code to create a Plane, Cube, Sphere, etc.
     */
    GameObject* CreatePrimitiveGameObject(Mesh *m, const String &name) const;

};

#endif // MENUBAR_H

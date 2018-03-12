#ifndef GLUNIFORMS_H
#define GLUNIFORMS_H

#include "Bang/GL.h"
#include "Bang/Array.h"
#include "Bang/Matrix4.h"
#include "Bang/Vector2.h"
#include "Bang/TypeMap.h"
#include "Bang/UniformBuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class ShaderProgram;

class GLUniforms
{
public:
    struct MatrixUniforms
    {
        Matrix4 model;
        Matrix4 normal;
        Matrix4 view;
        Matrix4 viewInv;
        Matrix4 proj;
        Matrix4 projInv;
        Matrix4 pvm;
    };

    struct CameraUniforms
    {
        float zNear;
        float zFar;
    };

    struct ViewportUniforms
    {
        Vector2 minPos;
        Vector2 size;
    };

    template <class T>
    struct GLSLVar
    {
        String name = ""; T value;
        GLSLVar(const String &_name, const T &_value)
            : name(_name), value(_value) {}
        GLSLVar() {}
    };

    template <class T>
    static T GetUniform(GLId program, int uniformLocation);

    template <class T>
    static T GetUniform(GLId program, const String &uniformName);

    template <class T>
    static T GetUniform(const String &uniformName);

    template <class T>
    static GLSLVar<T> GetUniformAt(GLId shaderProgramId, GLuint uniformIndex);

    static void SetAllUniformsToShaderProgram(ShaderProgram *sp);

    static void SetCameraUniforms(float zNear, float zFar);
    static void SetModelMatrix(const Matrix4 &model);
    static void SetViewMatrix(const Matrix4 &view);
    static void SetProjectionMatrix(const Matrix4 &projection);
    static void UpdatePVMMatrix();

    void SetViewProjMode(GL::ViewProjMode viewProjMode);
    GL::ViewProjMode GetViewProjMode() const;

    static const Matrix4 &GetModelMatrix();
    static const Matrix4 &GetViewMatrix();
    static       Matrix4  GetProjectionMatrix();

    static GLUniforms *GetActive();

private:
    ViewportUniforms m_viewportUniforms;
    MatrixUniforms m_matrixUniforms;
    CameraUniforms m_cameraUniforms;
    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::Canvas;

    GLUniforms() = default;
    virtual ~GLUniforms() = default;

    static void OnViewportChanged(const AARecti &newViewport);

    static ViewportUniforms* GetViewportUniforms();
    static MatrixUniforms *GetMatrixUniforms();
    static CameraUniforms *GetCameraUniforms();
    static Matrix4 GetCanvasProjectionMatrix();

    friend class GL;
};

NAMESPACE_BANG_END

#include "Bang/GLUniforms.tcc"

#endif // GLUNIFORMS_H


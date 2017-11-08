#ifndef GLUNIFORMS_H
#define GLUNIFORMS_H

#include "Bang/GL.h"
#include "Bang/Array.h"
#include "Bang/TypeMap.h"
#include "Bang/UniformBuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class ShaderProgram;

class GLUniforms
{
public:
    struct Matrices
    {
        Matrix4 model;
        Matrix4 normal;
        Matrix4 view;
        Matrix4 viewInv;
        Matrix4 proj;
        Matrix4 projInv;
        Matrix4 pvm;
    };

    struct Camera
    {
        float zNear;
        float zFar;
    };

    struct Viewport
    {
        Vector2 minPos;
        Vector2 size;
    };

    template <class T>
    static UniformBuffer<T>* CreateBuffer();

    template <class T>
    static UniformBuffer<T>* GetBuffer();

    template <class T>
    static void RemoveBuffer();

    static void BindAllUniformBuffersToShader(const ShaderProgram *sp);

    static UniformBuffer<Matrices>* GetMatricesBuffer();
    static UniformBuffer<Camera>* GetCameraBuffer();
    static UniformBuffer<Viewport>* GetViewportBuffer();

private:
    int m_bindingPointsUsed = 0;
    TypeMap<IUniformBuffer*> m_uniformBuffers;

    GLUniforms();
    virtual ~GLUniforms();

    template <class T>
    UniformBuffer<T>* _CreateBuffer();

    template <class T>
    UniformBuffer<T>* _GetBuffer();

    template <class T>
    void _RemoveBuffer();

    void RemoveBuffer(IUniformBuffer *buffer);

    static GLUniforms *GetActive();

    friend class GL;
};

NAMESPACE_BANG_END

#include "Bang/GLUniforms.tcc"

#endif // GLUNIFORMS_H


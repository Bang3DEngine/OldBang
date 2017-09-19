#ifndef GL_TCC
#define GL_TCC

#include "Bang/GL.h"

NAMESPACE_BANG_BEGIN

template <class T>
bool GL::Uniform(const String &uniformName, const T& value, bool warn)
{
    GLId shaderProgramId = GL::GetBoundId(GL::BindTarget::ShaderProgram);
    if (shaderProgramId <= 0)
    {
        if (warn)
        { Debug_Warn("Can't set uniform. No shader program currently bound."); }
        return false;
    }

    int location = GL::GetUniformLocation(shaderProgramId, uniformName);
    if (location <= 0)
    {
        if (warn)
        {
            Debug_Warn("Could not find uniform '" << uniformName << "' in the "
                       "current shader");
        }
        return false;
    }

    GL::Uniform(location, value);
    return true;
}

template <>
inline int GL::GetUniform(GLId program, int uniformLocation)
{
    int x[4]; glGetUniformiv(program, uniformLocation, x); return x[0];
}
template <>
inline float GL::GetUniform(GLId program, int uniformLocation)
{
    float x[4]; glGetUniformfv(program, uniformLocation, x); return x[0];
}
template <>
inline Vector2 GL::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v; glGetUniformfv(program, uniformLocation, &v.x); return v.xy();
}
template <>
inline Vector3 GL::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v; glGetUniformfv(program, uniformLocation, &v.x); return v.xyz();
}
template <>
inline Vector4 GL::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v; glGetUniformfv(program, uniformLocation, &v.x); return v;
}
template <>
inline Color GL::GetUniform(GLId program, int uniformLocation)
{
    Color c; glGetUniformfv(program, uniformLocation, &c.r); return c;
}

template <class T>
T GL::GetUniform(GLId program, const String &uniformName)
{
    return GL::GetUniform<T>(program,
                             GL::GetUniformLocation(program,
                                                    uniformName.ToCString()));
}

template <class T>
T GL::GetUniform(const String &uniformName)
{
    return GL::GetUniform<T>(GL::GetBoundId(GL::BindTarget::ShaderProgram),
                             uniformName);
}

NAMESPACE_BANG_END

#endif // GL_TCC

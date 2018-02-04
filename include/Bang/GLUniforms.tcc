#include "Bang/GLUniforms.h"

USING_NAMESPACE_BANG

template <class T>
T GLUniforms::GetUniform(GLId program, const String &uniformName)
{
    return GLUniforms::GetUniform<T>(program,
                                     GL::GetUniformLocation(program,
                                                            uniformName.ToCString()));
}

template <class T>
T GLUniforms::GetUniform(const String &uniformName)
{
    return GLUniforms::GetUniform<T>(GL::GetBoundId(GL::BindTarget::ShaderProgram),
                                     uniformName);
}

template<class T>
GLUniforms::GLSLVar<T> GLUniforms::GetUniformAt(GLId shaderProgramId,
                                                GLuint uniformIndex)
{
    if (shaderProgramId == 0) { return GLUniforms::GLSLVar<T>(); }

    GLint size;
    GLenum type;
    GLsizei length;
    constexpr GLsizei bufSize = 128;
    GLchar cname[bufSize];

    GL_CALL(
    glGetActiveUniform(shaderProgramId,
                       Cast<GLuint>(uniformIndex),
                       bufSize,
                       &length,
                       &size,
                       &type,
                       cname);
    );

    String name(cname);
    T uniformValue = GLUniforms::GetUniform<T>(shaderProgramId, name);
    return GLUniforms::GLSLVar<T>(name, uniformValue);
}


NAMESPACE_BANG_BEGIN

template <>
inline int GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    int x[4];
    GL_CALL( glGetUniformiv(program, uniformLocation, x) );
    return x[0];
}
template <>
inline short GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    return (GLUniforms::GetUniform<int>(program, uniformLocation) != 0);
}
template <>
inline Byte GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    return (GLUniforms::GetUniform<int>(program, uniformLocation) != 0);
}
template <>
inline bool GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    return (GLUniforms::GetUniform<int>(program, uniformLocation) != 0);
}
template <>
inline float GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    float x[4];
    GL_CALL( glGetUniformfv(program, uniformLocation, x) );
    return x[0];
}
template <>
inline Vector2 GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v;
    GL_CALL( glGetUniformfv(program, uniformLocation, &v.x) );
    return v.xy();
}
template <>
inline Vector3 GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v;
    GL_CALL( glGetUniformfv(program, uniformLocation, &v.x) );
    return v.xyz();
}
template <>
inline Vector4 GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Vector4 v;
    GL_CALL( glGetUniformfv(program, uniformLocation, &v.x) );
    return v;
}
template <>
inline Color GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Color c;
    GL_CALL( glGetUniformfv(program, uniformLocation, &c.r) );
    return c;
}
template <>
inline Matrix3 GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Matrix3 m;
    GL_CALL( glGetUniformfv(program, uniformLocation, m.Data()) );
    return m;
}
template <>
inline Matrix4 GLUniforms::GetUniform(GLId program, int uniformLocation)
{
    Matrix4 m;
    GL_CALL( glGetUniformfv(program, uniformLocation, m.Data()) );
    return m;
}

NAMESPACE_BANG_END

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

NAMESPACE_BANG_END

#endif // GL_TCC

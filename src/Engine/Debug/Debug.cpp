#include "Bang/Debug.h"

#include "Bang/GL.h"
#include "Bang/Path.h"
#include "Bang/Shader.h"
#include "Bang/String.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

USING_NAMESPACE_BANG

const String Debug::c_logPrefix    = "[   LOG   ]: ";
const String Debug::c_warnPrefix   = "[ WARNING ]: ";
const String Debug::c_errorPrefix  = "[  ERROR  ]: ";

Debug::Debug() { }


void Debug::Log(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_logPrefix << str << " | " <<
                 fileName << "(" << line << ")" <<  std::endl;
    std::cerr.flush();
}

void Debug::Warn(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_warnPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
}

void Debug::Error(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_errorPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
}

void Debug::PrintUniforms(Shader *shader)
{
    Debug::PrintUniforms(shader->GetGLId());
}

void Debug::PrintUniforms(uint programId)
{
    int uniformsCount = GL::GetUniformsListSize(programId);
    for (int i = 0; i < uniformsCount; ++i)
    {
        GL::DataType type = GL::GetUniformTypeAt(programId, i);
        if (type == GL::DataType::Byte)
        {
            GL::GLSLVar<Byte> var = GL::GetUniformAt<Byte>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Double)
        {
            GL::GLSLVar<float> var = GL::GetUniformAt<float>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Float)
        {
            GL::GLSLVar<float> var = GL::GetUniformAt<float>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Int)
        {
            GL::GLSLVar<int> var = GL::GetUniformAt<int>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Short)
        {
            GL::GLSLVar<short> var = GL::GetUniformAt<short>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedByte)
        {
            GL::GLSLVar<Byte> var = GL::GetUniformAt<Byte>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedShort)
        {
            GL::GLSLVar<short> var = GL::GetUniformAt<short>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedInt)
        {
            GL::GLSLVar<int> var = GL::GetUniformAt<int>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector2)
        {
            GL::GLSLVar<Vector2> var = GL::GetUniformAt<Vector2>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector3)
        {
            GL::GLSLVar<Vector3> var = GL::GetUniformAt<Vector3>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector4)
        {
            GL::GLSLVar<Vector4> var = GL::GetUniformAt<Vector4>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Matrix3)
        {
            GL::GLSLVar<Matrix3> var = GL::GetUniformAt<Matrix3>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Matrix4)
        {
            GL::GLSLVar<Matrix4> var = GL::GetUniformAt<Matrix4>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
    }
}

void Debug::PrintUniforms()
{
    Debug::PrintUniforms(GL::GetBoundId(GL::BindTarget::ShaderProgram));
}

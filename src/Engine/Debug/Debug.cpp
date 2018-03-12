#include "Bang/Debug.h"

#include "Bang/Path.h"
#include "Bang/Shader.h"
#include "Bang/String.h"
#include "Bang/Matrix3.h"
#include "Bang/Matrix4.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/GLUniforms.h"

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
            GLUniforms::GLSLVar<Byte> var = GLUniforms::GetUniformAt<Byte>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Double)
        {
            GLUniforms::GLSLVar<float> var = GLUniforms::GetUniformAt<float>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Float)
        {
            GLUniforms::GLSLVar<float> var = GLUniforms::GetUniformAt<float>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Int)
        {
            GLUniforms::GLSLVar<int> var = GLUniforms::GetUniformAt<int>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Short)
        {
            GLUniforms::GLSLVar<short> var = GLUniforms::GetUniformAt<short>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedByte)
        {
            GLUniforms::GLSLVar<Byte> var = GLUniforms::GetUniformAt<Byte>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedShort)
        {
            GLUniforms::GLSLVar<short> var = GLUniforms::GetUniformAt<short>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::UnsignedInt)
        {
            GLUniforms::GLSLVar<int> var = GLUniforms::GetUniformAt<int>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector2)
        {
            GLUniforms::GLSLVar<Vector2> var = GLUniforms::GetUniformAt<Vector2>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector3)
        {
            GLUniforms::GLSLVar<Vector3> var = GLUniforms::GetUniformAt<Vector3>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Vector4)
        {
            GLUniforms::GLSLVar<Vector4> var = GLUniforms::GetUniformAt<Vector4>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Matrix3)
        {
            GLUniforms::GLSLVar<Matrix3> var = GLUniforms::GetUniformAt<Matrix3>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
        else if (type == GL::DataType::Matrix4)
        {
            GLUniforms::GLSLVar<Matrix4> var = GLUniforms::GetUniformAt<Matrix4>(programId, i);
            Debug_Log(var.name << ": " << var.value);
        }
    }
}

void Debug::PrintAllUniforms()
{
    PrintUniforms();
    Debug_Peek( GLUniforms::GetModelMatrix() );
    Debug_Peek( GLUniforms::GetViewMatrix() );
    Debug_Peek( GLUniforms::GetProjectionMatrix() );
    Debug_Peek( GLUniforms::GetActive()->GetViewProjMode() );
}

void Debug::PrintUniforms()
{
    Debug::PrintUniforms(GL::GetBoundId(GL::BindTarget::ShaderProgram));
}

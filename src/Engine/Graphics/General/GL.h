#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Bang/Path.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/G_Image.h"
#include "Bang/Matrix4.h"

#define GL_CheckError() ( GL::CheckError(__LINE__, __FUNCTION__, __FILE__) )

typedef GLuint GLId;

class G_VAO;
class G_Texture;
class GLObject;
class GLContext;
class G_ShaderProgram;
class GL
{
public:
    enum class RenderMode
    {
        Points    = GL_POINTS,
        Lines     = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        Quads     = GL_QUADS
    };

    enum class CullMode
    {
        Front        = GL_FRONT,
        Back         = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK,
        None         = GL_NONE
    };

    enum class BindTarget
    {
        None           = 0,
        Texture2D      = GL_TEXTURE_2D,
        ShaderProgram  ,
        G_Framebuffer    = GL_FRAMEBUFFER,
        VAO            ,
        VBO
    };

    enum class ViewProjMode
    {
        UseBoth            = 0,
        OnlyFixAspectRatio = 1,
        IgnoreBoth         = 2
    };

    static void ClearError();
    static bool CheckError(int line = 0, const String &func = "",
                           const String &file = "");
    static bool CheckG_FramebufferError();

    static void ClearColorBuffer(const Color& clearColor = Color::Zero,
                                 bool clearR = true, bool clearG = true,
                                 bool clearB = true, bool clearA = true);
    static void ClearDepthBuffer(float clearDepth = 1.0f);
    static void ClearStencilBuffer();

    static void Enable (GLenum glEnum);
    static void Disable(GLenum glEnum);

    static void SetViewport(int x, int y, int width, int height);
    static void SetLineWidth(float lineWidth);

    static void SetViewProjMode(ViewProjMode mode);
    static void SetWriteDepth(bool writeDepth);
    static void SetTestDepth(bool testDepth);
    static void SetWireframe(bool wireframe);
    static void SetCullMode(const GL::CullMode cullMode);
    static void SetModelMatrix(const Matrix4 &model);
    static void SetViewMatrix(const Matrix4 &view);
    static void SetProjectionMatrix(const Matrix4 &projection);
    static void SetZNearFar(float zNear, float zFar);

    static void ApplyContextToShaderProgram(G_ShaderProgram *sp);
    static void Render(const G_VAO* vao,
                       GL::RenderMode renderMode,
                       int elementsCount,
                       int startElementIndex = 0);

    static bool IsWriteDepth();
    static bool IsTestDepth();
    static bool IsWireframe();
    static GL::CullMode GetCullMode();
    static const Matrix4 &GetModelMatrix();
    static const Matrix4 &GetViewMatrix();
    static const Matrix4 &GetProjectionMatrix();

    static void Bind(const GLObject *bindable);
    static void Bind(BindTarget bindTarget, GLId glId);
    static void UnBind(const GLObject *bindable);
    static void UnBind(BindTarget bindTarget);
    static bool IsBound(const GLObject *bindable);
    static bool IsBound(BindTarget bindTarget, GLId glId);

    static GLContext* GetGLContext();

private:
    GL();

    static void _Bind(BindTarget bindTarget, GLId glId);

    friend class GLContext;
};

#endif // GL_H

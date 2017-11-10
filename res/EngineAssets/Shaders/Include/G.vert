#define BANG_VERTEX
#define BANG_G
#include "Common.glsl"
#include "Ins.glsl"
#include "Outs.glsl"

void InitMain()
{
    B_FIn_Position = ( B_Matrices.Model * vec4(B_VIn_Position, 1) ).xyz;
    B_FIn_Normal   = normalize( mat3(B_Matrices.Normal) * B_VIn_Normal);
    B_FIn_Uv       = vec2(B_VIn_Uv.x, B_VIn_Uv.y) * B_UvMultiply;
    gl_Position    = B_Matrices.PVM * vec4(B_VIn_Position, 1);
}

void EndMain()
{

}

#include "Main.glsl"

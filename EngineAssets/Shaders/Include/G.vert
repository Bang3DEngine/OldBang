#define BANG_VERTEX
#define BANG_G
#include "Common.glsl"

void InitMain()
{
    B_FragIn_PositionWorld = B_Model * vec4(B_In_PositionObject, 1);

    B_FragIn_NormalWorld   = normalize(B_Normal * B_In_NormalObject);

    B_FragIn_Uv            = vec2(B_In_Uv.x, 1.0f - B_In_Uv.y);
    B_FragIn_Uv           *= B_UvMultiply;

    gl_Position            = B_PVM * vec4(B_In_PositionObject, 1);
}

void EndMain()
{
}

#include "Main.glsl"

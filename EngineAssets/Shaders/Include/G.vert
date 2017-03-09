#define BANG_VERTEX
#define BANG_G
#include "Common.glsl"

void InitMain()
{
    // Some default values
    B_FragIn_PositionWorld = B_Model  * vec4(B_In_PositionObject, 1);
    B_FragIn_NormalWorld   = B_Normal * vec4(B_In_NormalObject,   0);
    B_FragIn_Uv            = vec2(B_In_Uv.x, 1.0f - B_In_Uv.y);
    gl_Position            = B_PVM * vec4(B_In_PositionObject,   1);
}

void EndMain()
{
}

#include "Main.glsl"

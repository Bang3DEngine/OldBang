uniform mat4 B_Model;
uniform mat4 B_View;
uniform mat4 B_Projection;
uniform mat4 B_PVM;

in vec3 B_In_PositionObject;
in vec2 B_In_Uv;

out vec2 B_FragIn_Uv;
out vec4 B_FragIn_PositionWorld;

void main()
{
    B_FragIn_PositionWorld = B_Model * vec4(B_In_PositionObject, 1.0);
    B_FragIn_Uv = vec2(B_In_Uv.x, 1.0f - B_In_Uv.y);
    gl_Position = B_PVM * vec4(B_In_PositionObject, 1);
}

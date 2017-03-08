uniform mat4 B_Model;
uniform mat4 B_View;
uniform mat4 B_Projection;
uniform mat4 B_PVM;

in vec3 B_InPositionObject;
in vec2 B_InUv;

out vec2 B_uv_world_vout_fin;
out vec4 B_position_raw_vout_fin;

void main()
{
    B_position_raw_vout_fin = B_Model * vec4(B_InPositionObject, 1.0);
    B_uv_world_vout_fin = vec2(B_InUv.x, 1.0f - B_InUv.y);
    gl_Position = B_PVM * vec4(B_InPositionObject, 1.0);
}

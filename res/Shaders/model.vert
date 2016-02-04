#version 130

uniform mat4 BANG_matrix_model;

in vec3 BANG_position_vin;

out vec4 BANG_position_vout_fin;

void main()
{
    vec4 tVertex = BANG_matrix_model * vec4(BANG_position_vin, 1.0);
    gl_Position = tVertex;
}

#version 130

in vec3 vertexPos_vin;

out vec3 vertexPos_vout_fin;

void main()
{
    vertexPos_vout_fin = vertexPos_vin;
    gl_Position = vec4(vertexPos_vout_fin, 1.0);
}

#version 130

in vec3 vertexPos_in;

out vec3 vertexPos_out;

void main()
{
    vertexPos_out = vertexPos_in;
    gl_Position = vec4(vertexPos_out, 1.0);
}

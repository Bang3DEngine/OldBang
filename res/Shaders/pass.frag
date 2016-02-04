#version 130

in vec3 vertexPos_vout_fin;

void main()
{
    gl_FragColor = vec4(vertexPos_vout_fin,1);
}

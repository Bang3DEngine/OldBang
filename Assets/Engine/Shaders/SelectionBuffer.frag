#version 130

uniform vec3 selectionColor;

out vec4 fragColor;
out vec4 depth;

void main()
{
    fragColor = vec4(selectionColor * 0.01f + vec3(gl_FragCoord.z), 1.0);
    //fragColor = vec4(vec3(gl_FragCoord.z) + selectionColor * 0.01, 1.0);
    depth = vec4(gl_FragCoord.z);
}

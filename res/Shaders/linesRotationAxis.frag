#version 130

uniform mat4 BANG_matrix_view;
uniform vec3 BANG_position_camera;
uniform vec4 BANG_material_diffuse_color;
uniform vec3 wCircleCenter;
uniform float boundingSphereRadius;

in vec3 wPos;
in vec4 BANG_position_raw_vout_fin;

out vec3 BANG_position_fout_gbufferin;
out vec3 BANG_normal_fout_gbufferin;
out vec2 BANG_uv_fout_gbufferin;
out vec3 BANG_diffuse_fout_gbufferin;
out float BANG_depth_fout_gbufferin;

void main()
{
    vec3 wCameraUp = normalize( (BANG_matrix_view * vec4(0,1,0,0)).xyz );
    vec3 wMaxDistPoint = wCircleCenter + wCameraUp * boundingSphereRadius;

    float maxDist = distance(BANG_position_camera, wMaxDistPoint);
    float dToV = distance(BANG_position_camera, wPos);

    //BANG_diffuse_fout_gbufferin = vec3(dot(vec3(0,1,0), normalize(wPos-wCircleCenter)));
    //if(dToV > maxDist) discard;


    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
    BANG_normal_fout_gbufferin = vec3(0.0f);
    BANG_uv_fout_gbufferin = vec2(0.0f);
    BANG_diffuse_fout_gbufferin = BANG_material_diffuse_color.rgb;
    BANG_depth_fout_gbufferin = gl_FragCoord.z;
}

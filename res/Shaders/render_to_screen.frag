#version 130
#define BANG_EDITOR

uniform sampler2D BANG_texture_0; //positions
uniform sampler2D BANG_texture_1; //normals
uniform sampler2D BANG_texture_2; //uvs
uniform sampler2D BANG_texture_3; //diffuse
uniform sampler2D BANG_texture_4; //depth
uniform sampler2D BANG_texture_5; //selectedInEditor

in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec3  BANG_position = texture2D(BANG_texture_0, BANG_uv_raw_vout_fin).xyz;
    vec3  BANG_normal   = texture2D(BANG_texture_1, BANG_uv_raw_vout_fin).xyz;
    vec2  BANG_uv       = texture2D(BANG_texture_2, BANG_uv_raw_vout_fin).xy;
    vec3  BANG_diffuse  = texture2D(BANG_texture_3, BANG_uv_raw_vout_fin).rgb;
    float BANG_depth    = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin).x ;

    #ifdef BANG_EDITOR
    float BANG_selectedInEditor = texture2D(BANG_texture_5, BANG_uv_raw_vout_fin).x ;
    float BANG_selectedInEditor1 = texture2D(BANG_texture_5, BANG_uv_raw_vout_fin + vec2(0,0.002)).x;
    float BANG_selectedInEditor2 = texture2D(BANG_texture_5, BANG_uv_raw_vout_fin + vec2(0.002,0)).x;
    float BANG_selectedInEditor3 = texture2D(BANG_texture_5, BANG_uv_raw_vout_fin + vec2(-0.002,0)).x;
    float BANG_selectedInEditor4 = texture2D(BANG_texture_5, BANG_uv_raw_vout_fin + vec2(0,-0.002)).x;
    #endif

    float depthUp = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin + vec2(0,0.001)).x;
    float depthRight = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin + vec2(0.001,0)).x;
    float depthLeft = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin + vec2(-0.001,0)).x;
    float depthDown = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin + vec2(0,-0.001)).x;

    if(abs(BANG_depth - depthUp) > 0.01 ||
       abs(BANG_depth - depthRight) > 0.01 ||
       abs(BANG_depth - depthLeft) > 0.01 ||
       abs(BANG_depth - depthDown) > 0.01 )
    {
        if(BANG_selectedInEditor + BANG_selectedInEditor1 +
           BANG_selectedInEditor2 +  BANG_selectedInEditor3 +
           BANG_selectedInEditor4 > 0.5)
        {
            gl_FragColor = vec4(1.0,0.3,0,1);
        }
        else
        {
            gl_FragColor = vec4(0,0,0,1);
        }
    }
    else if(BANG_depth < 1.0)
    {
        gl_FragColor = vec4(BANG_diffuse, 1);
    }
    else
    {
        gl_FragColor = vec4(vec3(0.2,0.2,0.6) + (BANG_uv_raw_vout_fin.y) * vec3(0.8,0.8,0.4), 1.0);
    }
}

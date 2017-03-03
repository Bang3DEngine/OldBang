#define BANG_FRAGMENT
#define BANG_G
#include "Uniforms.glsl"

struct B_VertexOut_GBufferIn   // GBuffer
{
    vec3 normal_world;
    vec4 diffuseColor;
    vec4 color;
    bool receivesLighting;
    float shininess;
    float depth;
    float stencil;
};

in vec4 B_position_world_vout_fin;
in vec4 B_normal_world_vout_fin;
in vec2 B_uv_vout_fin;

out vec4 B_normal_fout_gin;
out vec4 B_diffuse_fout_gin;
out vec4 B_misc_fout_gin;
out vec4 B_color_fout_gin;

B_VertexOut_GBufferIn B_vout;

void InitMain()
{
    // Some default values
    B_vout.normal_world        = FRAG_IN_NORMAL_WORLD().xyz;
    B_vout.receivesLighting    = B_material_receivesLighting;
    B_vout.shininess           = B_material_shininess;
    B_vout.diffuseColor        = B_material_diffuse_color;
    B_vout.depth               = gl_FragCoord.z;
    B_vout.stencil             = 1;
    B_vout.color               = B_material_diffuse_color;
}

void EndMain()
{
    B_normal_fout_gin                = vec4(B_vout.normal_world, 0);
    B_diffuse_fout_gin               = B_vout.diffuseColor;
    B_misc_fout_gin.r = B_vout.receivesLighting ? 1 : 0;
    B_misc_fout_gin.g = B_vout.shininess;
    B_misc_fout_gin.b = B_vout.depth;
    if (B_stencilWriteEnabled) { B_misc_fout_gin.a = B_vout.stencil; }

    float ambientLight = (B_material_receivesLighting ? 0.1 : 1.0);
    vec3 outColor = ambientLight * B_vout.diffuseColor.rgb;
    B_vout.color = vec4( mix(FRAG_IN_COLOR().rgb, outColor, B_vout.color.a), 1);
    B_color_fout_gin = B_vout.color;
}

#include "Main.glsl"

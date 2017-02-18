#define BANG_FRAGMENT
#define BANG_D2G
#include "Uniforms.glsl"

struct B_VertexOut_GBufferIn   // GBuffer
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
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

out vec4 B_position_fout_gin;
out vec4 B_normal_fout_gin;
out vec4 B_uv_fout_gin;
out vec4 B_diffuse_fout_gin;
out vec4 B_materialProps_fout_gin;
out vec4 B_depth_fout_gin;
out vec4 B_stencil_fout_gin;
out vec4 B_color_fout_gin;

B_VertexOut_GBufferIn B_vout;

void InitMain()
{
    // Some default values
    B_vout.position_world      = FRAG_IN_POSITION_WORLD().xyz;
    B_vout.normal_world        = FRAG_IN_NORMAL_WORLD().xyz;
    B_vout.uv                  = FRAG_IN_UV();
    B_vout.receivesLighting    = B_material_receivesLighting;
    B_vout.shininess           = B_material_shininess;
    B_vout.diffuseColor        = B_material_diffuse_color;
    B_vout.depth               = gl_FragCoord.z;
    B_vout.stencil             = 1;
    B_vout.color               = B_material_diffuse_color;
}

void EndMain()
{
    B_position_fout_gin              = vec4(B_vout.position_world, 1);
    B_normal_fout_gin                = vec4(B_vout.normal_world, 0);
    B_uv_fout_gin                    = vec4(B_vout.uv, 0, 0);
    B_diffuse_fout_gin               = B_vout.diffuseColor;
    B_materialProps_fout_gin.x       = B_vout.receivesLighting ? 1.0f : 0.0f;
    B_materialProps_fout_gin.y       = B_vout.shininess;
    B_materialProps_fout_gin.z       = B_vout.shininess;
    B_materialProps_fout_gin.w       = B_gameObject_isSelected ? 1.0f : 0.0f; // Editor
    B_depth_fout_gin                 = vec4(B_vout.depth);

    if (B_stencilWriteEnabled)
    {
        B_stencil_fout_gin = vec4(B_vout.stencil);
    }

    float ambientLight = (B_material_receivesLighting ? 0.1 : 1.0);
    vec3 outColor = ambientLight * B_vout.diffuseColor.rgb;
    B_vout.color = vec4( mix(FRAG_IN_COLOR().rgb, outColor, B_vout.color.a), 1);
    B_color_fout_gin = B_vout.color;
}

#include "Main.glsl"

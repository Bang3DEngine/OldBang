#include "Version.glsl"
#include "Uniforms.glsl"

struct B_VertexIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv;
};

struct B_VertexOut_GBufferIn   // GBuffer
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec4 diffuseColor;
    bool receivesLighting;
    float shininess;
    float depth;
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
out vec4 B_color_fout_gin;

B_VertexIn B_vin;
B_VertexOut_GBufferIn B_vout;

void InitMain()
{
    // Fill vin
    B_vin.position_world = B_position_world_vout_fin;
    B_vin.normal_world   = B_normal_world_vout_fin;
    B_vin.uv             = B_uv_vout_fin;

    // Some default values
    B_vout.position_world      = B_vin.position_world.xyz;
    B_vout.normal_world        = B_vin.normal_world.xyz;
    B_vout.uv                  = B_vin.uv;
    B_vout.receivesLighting    = B_renderer_receivesLighting > 0.5;
    B_vout.shininess           = B_material_shininess;
    B_vout.diffuseColor        = B_material_diffuse_color.rgba;
    B_vout.depth               = gl_FragCoord.z;
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
    B_materialProps_fout_gin.w       = B_gameObject_isSelected; // Editor
    B_depth_fout_gin                 = vec4(B_vout.depth);

    B_color_fout_gin = vec4(0);
}

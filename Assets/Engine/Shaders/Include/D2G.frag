#define BANG_FRAGMENT
#include "Uniforms.glsl"

struct B_VertexIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv;
    vec4 diffuseColor; // Diffuse component
    float depth;
    float stencil;
    vec4 color;        // Color (with PR applied)
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

B_VertexIn B_vin;
B_VertexOut_GBufferIn B_vout;

void InitMain()
{
    // Fill vin
    B_vin.position_world = B_position_world_vout_fin;
    B_vin.normal_world   = B_normal_world_vout_fin;
    B_vin.uv             = B_uv_vout_fin;
    B_vin.diffuseColor   = texture2D(B_diffuse_gout_fin, B_screen_coord_norm);
    B_vin.depth          = texture2D(B_depth_gout_fin,   B_screen_coord_norm).x;
    B_vin.stencil        = texture2D(B_stencil_gout_fin, B_screen_coord_norm).x;
    B_vin.color          = texture2D(B_color_gout_fin,   B_screen_coord_norm);

    // Some default values
    B_vout.position_world      = B_vin.position_world.xyz;
    B_vout.normal_world        = B_vin.normal_world.xyz;
    B_vout.uv                  = B_vin.uv;
    B_vout.receivesLighting    = B_renderer_receivesLighting;
    B_vout.shininess           = B_material_shininess;
    B_vout.diffuseColor        = B_material_diffuse_color;
    B_vout.depth               = gl_FragCoord.z;
    B_vout.stencil             = 1;
}

void EndMain()
{
    B_position_fout_gin              = vec4(B_vout.position_world, 1);
    B_normal_fout_gin                = vec4(B_vout.normal_world, 0);
    B_uv_fout_gin                    = vec4(B_vout.uv, 0, 0);
    B_diffuse_fout_gin               = B_vout.diffuseColor;
    //vec4( mix(B_diffuse_fout_gin.rgb,
      //                                           B_vout.diffuseColor.rgb,
        //                                         B_vout.diffuseColor.a * B_diffuse_fout_gin.a),
          //                                       B_vout.diffuseColor.a);

    B_materialProps_fout_gin.x       = B_vout.receivesLighting ? 1.0f : 0.0f;
    B_materialProps_fout_gin.y       = B_vout.shininess;
    B_materialProps_fout_gin.z       = B_vout.shininess;
    B_materialProps_fout_gin.w       = B_gameObject_isSelected ? 1.0f : 0.0f; // Editor
    B_depth_fout_gin                 = vec4(B_vout.depth);

    if (B_stencilWriteEnabled)
    {
        B_stencil_fout_gin = vec4(B_vout.stencil);
    }
}

#include "Main.glsl"

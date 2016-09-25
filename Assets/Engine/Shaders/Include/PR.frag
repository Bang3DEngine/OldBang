#include "Version.glsl"
#define BANG_FRAGMENT
#include "Uniforms.glsl"

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec4 diffuseColor;
    float shininess;
    bool receivesLighting;
    float depth;
    float stencil;
    vec4 color;

    vec2 uv_screen;
};

struct B_VertexOut
{
    vec4 color;
};

B_VertexIn B_vin;
B_VertexOut B_vout;

out vec4 B_color_gout_gin; // Accumulated color

bool PassStencil()
{
    return !B_stencilTestEnabled || B_vin.stencil == 1;
}

void InitMain()
{
    B_vin.uv_screen           = B_screen_coord_norm;
    B_vin.color               = texture2D(B_color_gout_fin,          B_vin.uv_screen);
    B_vin.position_world      = texture2D(B_position_gout_fin,       B_vin.uv_screen).xyz;
    B_vin.normal_world        = texture2D(B_normal_gout_fin,         B_vin.uv_screen).xyz;
    B_vin.uv                  = texture2D(B_uv_gout_fin,             B_vin.uv_screen).xy;
    B_vin.diffuseColor        = texture2D(B_diffuse_gout_fin,        B_vin.uv_screen);
    B_vin.receivesLighting    = texture2D(B_materialProps_gout_fin,  B_vin.uv_screen).x > 0.5f;
    B_vin.shininess           = texture2D(B_materialProps_gout_fin,  B_vin.uv_screen).y;
    B_vin.depth               = texture2D(B_depth_gout_fin,          B_vin.uv_screen).x;
    B_vin.stencil             = texture2D(B_stencil_gout_fin,        B_vin.uv_screen).x;
    B_vin.normal_world        = normalize(B_vin.normal_world);

    // Default value
    B_vout.color              = B_vin.color;
}

bool IsEmpty()
{
    return B_vin.depth == 1.0f;
}

void EndMain()
{
    B_vout.color = vec4( mix(B_vin.color.rgb, B_vout.color.rgb, B_vout.color.a), 1 );
    B_color_gout_gin = B_vout.color;
}

// Custom main that checks stencil for this case
void Main();
void main()
{
    InitMain();

    if (PassStencil())
    {
        Main();
    }

    EndMain();
}


B_POST_RENDER_FS_DECLARE();

uniform float B_light_intensity;
uniform vec3  B_light_color;
uniform vec3  B_light_forward_world;
uniform vec3  B_light_position_world;

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec3 diffuseColor;
    float receivesLighting;
    float depth;

    vec2 uv_screen;
};

struct B_VertexOut
{
    vec4 color;
};

B_VertexIn B_vin;
B_VertexOut B_vout;

void main()
{
    B_POST_RENDER_FS_INIT_MAIN();

    if(B_vin.receivesLighting > 0.5f)
    {
        float lightDot = dot(B_vin.normal_world, normalize(B_light_forward_world));
        B_vout.color = vec4(B_vin.diffuseColor *
                            lightDot *
                            B_light_intensity *
                            B_light_color, 1);
    }
    else
    {
        B_vout.color = vec4(B_vin.diffuseColor, 1);
    }

    B_POST_RENDER_FS_END_MAIN();
}

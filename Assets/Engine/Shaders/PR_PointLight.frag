B_POST_RENDER_FS_DECLARE();

uniform float B_light_intensity;
uniform float B_light_range;
uniform vec4  B_light_color;
uniform vec3  B_light_position_world;

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec4 diffuseColor;
    bool receivesLighting;
    float shininess;
    float depth;
    vec4 color;

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

    if (B_vin.receivesLighting)
    {
        vec3 dir = normalize(B_light_position_world - B_vin.position_world);
        float lightDot = max(0.0, dot(B_vin.normal_world, dir));

        // Linear Attenuation
        float d = distance(B_light_position_world, B_vin.position_world) ;
        float linearAtt = 1.0f - d / B_light_range; // 0 is light's pos, 1 is max range
        linearAtt = clamp(linearAtt, 0.0, 1.0);

        float intensityAtt = B_light_intensity * linearAtt;

        // DIFFUSE
        vec3 diffLight = B_vin.diffuseColor.rgb * lightDot * intensityAtt * B_light_color.rgb;

        // SPECULAR
        vec3 worldCamPos     = B_position_camera.xyz;
        vec3 pointToCamDir   = normalize(worldCamPos - B_vin.position_world);
        vec3 lightToPointDir = normalize(B_light_position_world - B_vin.position_world);
        vec3 reflected       = normalize( -reflect(lightToPointDir, B_vin.normal_world) );
        float specDot        = max(0.0, dot(reflected, pointToCamDir));
        float specShin       = pow(specDot, B_vin.shininess);
        specShin = clamp(specShin, 0.0, 1.0);
        vec3 lightSpecular   = specShin * lightDot * intensityAtt * B_light_intensity * B_light_color.rgb;

        // TOTAL SUM
        B_vout.color = vec4(B_vin.color.rgb +
                            diffLight + lightSpecular,
                            B_vin.diffuseColor.a);
    }

    B_POST_RENDER_FS_END_MAIN();
}

// Matrices
uniform mat4 B_Model;
uniform mat4 B_ModelInv;
uniform mat4 B_Normal;
uniform mat4 B_NormalInv;
uniform mat4 B_View;
uniform mat4 B_ViewInv;
uniform mat4 B_Projection;
uniform mat4 B_ProjectionInv;
uniform mat4 B_PVM;

// Camera
uniform vec3 B_CameraPositionWorld;

// Stencil
uniform bool B_StencilWriteEnabled;
uniform bool B_StencilTestEnabled;

// Material related
uniform bool  B_MaterialReceivesLighting;
uniform vec4  B_MaterialDiffuseColor;
uniform float B_AlphaCutoff = -1.0f;
uniform float B_MaterialShininess;
uniform bool  B_HasTexture = false;
uniform sampler2D B_Texture0;

// Screen related
uniform vec2 B_ScreenSize;

#define B_GTEX_NORMAL    B_normal_gout_fin
#define B_GTEX_DIFFCOLOR B_diffuse_gout_fin
#define B_GTEX_MISC      B_misc_gout_fin
#define B_GTEX_COLOR     B_color_gout_fin

#ifdef BANG_FRAGMENT
vec2  B_ScreenStep = 1.0 / B_ScreenSize;
vec2  B_ScreenPos  = gl_FragCoord.xy;
vec2  B_ScreenUv   = B_ScreenPos / B_ScreenSize;
#endif

// Light related
uniform float B_light_range;
uniform vec4  B_LightColor;
uniform float B_LightIntensity;
uniform vec3  B_LightForwardWorld;
uniform vec3  B_LightPositionWorld;

// GBuffer textures
uniform sampler2D B_GTEX_NORMAL;
uniform sampler2D B_GTEX_DIFFCOLOR;
uniform sampler2D B_GTEX_MISC;
uniform sampler2D B_GTEX_COLOR;

// In/Out Structures

// Getters
#ifdef BANG_VERTEX
#endif

#ifdef BANG_FRAGMENT
    vec4  B_SampleColor(vec2 uv) { return texture2D(B_GTEX_COLOR, uv); }
    vec3  B_SampleNormal(vec2 uv) { return texture2D(B_GTEX_NORMAL, uv).xyz; }
    vec4  B_SampleDiffColor(vec2 uv) { return texture2D(B_GTEX_DIFFCOLOR, uv); }
    bool  B_SampleReceivesLight (vec2 uv) { return texture2D(B_GTEX_MISC, uv).r > 0.5; }
    float B_SampleShininess (vec2 uv) { return texture2D(B_GTEX_MISC, uv).g; }
    float B_SampleDepth(vec2 uv) { return texture2D(B_GTEX_MISC, uv).b; }
    float B_SampleStencil (vec2 uv) { return texture2D(B_GTEX_MISC, uv).a; }

    vec4  B_SampleColor()  { return B_SampleColor(B_ScreenUv); }
    vec3  B_SampleNormal() { return B_SampleNormal(B_ScreenUv); }
    vec4  B_SampleDiffColor() { return B_SampleDiffColor(B_ScreenUv); }
    bool  B_SampleReceivesLight() { return B_SampleReceivesLight(B_ScreenUv); }
    float B_SampleShininess () { return B_SampleShininess(B_ScreenUv); }
    float B_SampleDepth() { return B_SampleDepth(B_ScreenUv); }
    float B_SampleStencil() { return B_SampleStencil(B_ScreenUv); }

    vec3 B_ComputeWorldPosition(float depth)
    {
        float x = B_ScreenUv.x * 2.0 - 1.0;
        float y = B_ScreenUv.y * 2.0 - 1.0;
        float z = depth * 2.0 - 1.0;
        vec4 projectedPos = vec4(x, y, z, 1);
        vec4 worldPos = (B_ProjectionInv * projectedPos);
        worldPos = (B_ViewInv * (worldPos/worldPos.w));
        return worldPos.xyz;
    }
    vec3 B_ComputeWorldPosition() { return B_ComputeWorldPosition( B_SampleDepth() ); }
#endif



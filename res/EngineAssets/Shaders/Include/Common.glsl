layout (std140)
uniform MatricesBlock
{
    mat4 Model;
    mat4 Normal;
    mat4 View;
    mat4 ViewInv;
    mat4 Projection;
    mat4 ProjectionInv;
    mat4 PVM;
}
B_Matrices;

layout (std140)
uniform CameraBlock
{
    float ZNear;
    float ZFar;
}
B_Camera;

layout (std140)
uniform ViewportBlock
{
    vec2 MinPos;
    vec2 Size;
}
B_Viewport;


// Material related /////////////////////////
uniform bool  B_MaterialReceivesLighting;
uniform vec4  B_MaterialDiffuseColor;
uniform float B_AlphaCutoff = -1.0f;
uniform float B_MaterialShininess;
uniform vec2  B_UvMultiply = vec2(1);
uniform bool  B_HasTexture = false;
uniform sampler2D B_Texture0;
// ///////////////////////////////////////



// Screen related /////////////////////////
#ifdef BANG_FRAGMENT
vec2 B_ViewportStep;
vec2 B_ViewportPos;
vec2 B_ViewportUv;
#endif
// ///////////////////////////////////////



// Light related /////////////////////////
uniform float B_LightRange;
uniform vec4  B_LightColor;
uniform float B_LightIntensity;
uniform vec3  B_LightForwardWorld;
uniform vec3  B_LightPositionWorld;
// ///////////////////////////////////////


// GBuffer textures //////////////////////
uniform sampler2D  B_GTex_Normal;
uniform sampler2D  B_GTex_DiffColor;
uniform sampler2D  B_GTex_Misc;
uniform sampler2D  B_GTex_Color;
uniform sampler2D  B_GTex_DepthStencil;
// ///////////////////////////////////////


// Util functions /////////////////
float B_LinearizeDepth(float d)
{
    return (2 * B_Camera.ZNear) /
                (B_Camera.ZFar + B_Camera.ZNear -
                  d * (B_Camera.ZFar - B_Camera.ZNear));
}
//

// GBuffer Samplers //////////////////////
#ifdef BANG_FRAGMENT
    vec4  B_SampleColor(vec2 uv) { return texture2D(B_GTex_Color, uv); }
    vec3  B_SampleNormal(vec2 uv)
    {
        return texture2D(B_GTex_Normal, uv).xyz * 2.0f - 1.0f;
    }
    vec4  B_SampleDiffColor(vec2 uv) { return texture2D(B_GTex_DiffColor, uv); }
    bool  B_SampleReceivesLight (vec2 uv) { return texture2D(B_GTex_Misc, uv).r > 0; }
    float B_SampleShininess (vec2 uv) { return texture2D(B_GTex_Misc, uv).g; }
    float B_SampleDepth(vec2 uv) { return texture2D(B_GTex_DepthStencil, uv).r; }
    float B_SampleFlags(vec2 uv) { return texture2D(B_GTex_Misc, uv).z; }

    vec4  B_SampleColor()  { return B_SampleColor(B_ViewportUv); }
    vec3  B_SampleNormal() { return B_SampleNormal(B_ViewportUv); }
    vec4  B_SampleDiffColor() { return B_SampleDiffColor(B_ViewportUv); }
    bool  B_SampleReceivesLight() { return B_SampleReceivesLight(B_ViewportUv); }
    float B_SampleShininess () { return B_SampleShininess(B_ViewportUv); }
    float B_SampleDepth() { return B_SampleDepth(B_ViewportUv); }
    float B_SampleFlags() { return B_SampleFlags(B_ViewportUv); }

    vec4  B_SampleColorOffset(vec2 pixOffset)
      { return B_SampleColor(B_ViewportUv + B_ViewportStep * pixOffset); }
    vec3  B_SampleNormalOffset(vec2 pixOffset)
      { return B_SampleNormal(B_ViewportUv + B_ViewportStep * pixOffset); }
    vec4  B_SampleDiffColorOffset(vec2 pixOffset)
      { return B_SampleDiffColor(B_ViewportUv + B_ViewportStep * pixOffset); }
    bool  B_SampleReceivesLightOffset(vec2 pixOffset)
      { return B_SampleReceivesLight(B_ViewportUv + B_ViewportStep * pixOffset); }
    float B_SampleShininessOffset(vec2 pixOffset)
      { return B_SampleShininess(B_ViewportUv + B_ViewportStep * pixOffset); }
    float B_SampleDepthOffset(vec2 pixOffset)
      { return B_SampleDepth(B_ViewportUv + B_ViewportStep * pixOffset); }
    float B_SampleFlagsOffset(vec2 pixOffset)
      { return B_SampleFlags(B_ViewportUv + B_ViewportStep * pixOffset); }

    vec3 B_GetCameraPositionWorld() { return B_Matrices.ViewInv[3].xyz; }

    vec3 B_ComputeWorldPosition(float depth)
    {
        float x = B_ViewportUv.x * 2.0 - 1.0;
        float y = B_ViewportUv.y * 2.0 - 1.0;
        float z = depth * 2.0 - 1.0;
        vec4 projectedPos = vec4(x, y, z, 1);
        vec4 worldPos = (B_Matrices.ProjectionInv * projectedPos);
        worldPos = (B_Matrices.ViewInv * (worldPos/worldPos.w));
        return worldPos.xyz;
    }
    vec3 B_ComputeWorldPosition() { return B_ComputeWorldPosition( B_SampleDepth() ); }
#endif

void InitCommon()
{
    #ifdef BANG_FRAGMENT
    B_ViewportStep = 1.0 / B_Viewport.Size;
    B_ViewportPos  = (gl_FragCoord.xy - B_Viewport.MinPos);
    B_ViewportUv   = B_ViewportPos / B_Viewport.Size;
    #endif
}
// ///////////////////////////////////////



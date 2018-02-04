uniform mat4 B_Model;
uniform mat4 B_Normal;
uniform mat4 B_View;
uniform mat4 B_Projection;
uniform mat4 B_PVM;

uniform float B_Camera_ZNear;
uniform float B_Camera_ZFar;

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
uniform float B_AlphaCutoff;
uniform float B_MaterialShininess;
uniform vec2  B_UvOffset;
uniform vec2  B_UvMultiply;
uniform bool  B_HasTexture;
uniform sampler2D B_Texture0;
// ///////////////////////////////////////

// Light related /////////////////////////
uniform float B_LightRange;
uniform vec4  B_LightColor;
uniform float B_LightIntensity;
uniform vec3  B_LightForwardWorld;
uniform vec3  B_LightPositionWorld;
// ///////////////////////////////////////


// GBuffer textures //////////////////////
uniform sampler2D B_GTex_Normal;
uniform sampler2D B_GTex_DiffColor;
uniform sampler2D B_GTex_Misc;
uniform sampler2D B_GTex_Color;
uniform sampler2D B_GTex_DepthStencil;
// ///////////////////////////////////////

// Util functions /////////////////
float B_LinearizeDepth(float d)
{
    return (2 * B_Camera_ZNear) /
                (B_Camera_ZFar + B_Camera_ZNear -
                  d * (B_Camera_ZFar - B_Camera_ZNear));
}
#ifdef BANG_FRAGMENT
vec2 GetViewportStep() { return 1.0 / B_Viewport.Size; }
vec2 GetViewportPos() { return gl_FragCoord.xy - B_Viewport.MinPos; }
vec2 GetViewportUv() { return GetViewportPos() / B_Viewport.Size; }
#endif
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

vec4  B_SampleColor()  { return B_SampleColor(GetViewportUv()); }
vec3  B_SampleNormal() { return B_SampleNormal(GetViewportUv()); }
vec4  B_SampleDiffColor() { return B_SampleDiffColor(GetViewportUv()); }
bool  B_SampleReceivesLight() { return B_SampleReceivesLight(GetViewportUv()); }
float B_SampleShininess () { return B_SampleShininess(GetViewportUv()) * 255.0f; }
float B_SampleDepth() { return B_SampleDepth(GetViewportUv()); }
float B_SampleFlags() { return B_SampleFlags(GetViewportUv()); }

vec4  B_SampleColorOffset(vec2 pixOffset)
  { return B_SampleColor(GetViewportUv() + GetViewportStep() * pixOffset); }
vec3  B_SampleNormalOffset(vec2 pixOffset)
  { return B_SampleNormal(GetViewportUv() + GetViewportStep() * pixOffset); }
vec4  B_SampleDiffColorOffset(vec2 pixOffset)
  { return B_SampleDiffColor(GetViewportUv() + GetViewportStep() * pixOffset); }
bool  B_SampleReceivesLightOffset(vec2 pixOffset)
  { return B_SampleReceivesLight(GetViewportUv() + GetViewportStep() * pixOffset); }
float B_SampleShininessOffset(vec2 pixOffset)
  { return B_SampleShininess(GetViewportUv() + GetViewportStep() * pixOffset); }
float B_SampleDepthOffset(vec2 pixOffset)
  { return B_SampleDepth(GetViewportUv() + GetViewportStep() * pixOffset); }
float B_SampleFlagsOffset(vec2 pixOffset)
  { return B_SampleFlags(GetViewportUv() + GetViewportStep() * pixOffset); }

vec3 B_GetCameraPositionWorld() { return inverse(B_View)[3].xyz; }

vec3 B_ComputeWorldPosition(float depth)
{
    float x = GetViewportUv().x * 2.0 - 1.0;
    float y = GetViewportUv().y * 2.0 - 1.0;
    float z = depth * 2.0 - 1.0;
    vec4 projectedPos = vec4(x, y, z, 1);
    vec4 worldPos = (inverse(B_Projection) * projectedPos);
    worldPos = (inverse(B_View) * (worldPos/worldPos.w));
    return worldPos.xyz;
}
vec3 B_ComputeWorldPosition() { return B_ComputeWorldPosition( B_SampleDepth() ); }
#endif
// ///////////////////////////////////////


